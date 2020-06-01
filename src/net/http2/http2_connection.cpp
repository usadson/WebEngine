/**
 * Copyright (C) 2020 Tristan <tristan@thewoosh.me>
 *
 * All Rights Reserved.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
 */
#include "http2_connection.hpp" 

#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

#include <cstring>

#include "constants.hpp"
#include "logger.hpp"

namespace Net {
	static const char HTTP2_PREFACE[] = "PRI * HTTP/2.0\r\n\r\nSM\r\n\r\n";

	namespace HTTP {
		HTTP2Connection::HTTP2Connection(Net::ConnectionInfo *connectionInfo)
				: ConnectionInfo(connectionInfo) {
			if (!connectionInfo->Write(HTTP2_PREFACE, sizeof(HTTP2_PREFACE)/sizeof(HTTP2_PREFACE[0])-1)) {
				Logger::Warning(__PRETTY_FUNCTION__, "Failed to send preface.");
				return;
			}

			H2::Frame settingsFrame;
			settingsFrame.length = 0;
			settingsFrame.type = H2::FrameType::SETTINGS;
			settingsFrame.flags = 0;
			settingsFrame.stream = 0;
			SendFrame(settingsFrame);
		}

		HTTP2Connection::~HTTP2Connection() {
			
		}

		HTTP2Error HTTP2Connection::Request(HTTPResponseInfo *response, std::string method, std::string path) {
			if (!ConnectionInfo->Connected ||
				(ConnectionInfo->Secure && !ConnectionInfo->IsAuthenticated)) {
				return HTTP2Error::NOT_CONNECTED;
			}

			H2::Frame frame;
			while (true) {
				try {
					frame = ReadFrame();
					switch (frame.type) {
						case H2::FrameType::SETTINGS:
							HandleFrameSettings(frame);
							break;
						case H2::FrameType::GOAWAY:
							HandleFrameGoaway(frame);
							break;
						default:
							std::cout << "[UNKNOWN FRAME] Information about the frame:\n\tLength: " << frame.length << "\n\tType: " << (uint16_t)frame.type << "\n\tFlags: " << (uint16_t)frame.flags << "\n\tStream: " << frame.stream << std::endl;
							break;
					}
				} catch (const H2::Exception &exception) {
					Logger::Error(__PRETTY_FUNCTION__, exception.message);
				}
			}

			return HTTP2Error::NO_ERROR;
		}

		HTTP2Error HTTP2Connection::RequestNavigation(HTTPResponseInfo *response, std::string path) {
			return Request(response, "GET", path);
		}

		bool HTTP2Connection::SendFrame(H2::Frame frame) {
			/* Using a vector seems a bit overkill, since we can just use a
			 * malloc'ed array or something, since a vector adds a lot of
			 * abstraction we don't need; only the operator[] and memcpy 
			 * functions are needed. */
			std::vector<char> buf(9 + frame.payload.size());

			buf[0] = (uint8_t) frame.length >> 16;
			buf[1] = (uint8_t) frame.length >> 8;
			buf[2] = (uint8_t) frame.length;
			buf[3] = frame.type;
			buf[4] = frame.flags;
			buf[5] = (uint8_t) frame.stream >> 24;
			buf[6] = (uint8_t) frame.stream >> 16;
			buf[7] = (uint8_t) frame.stream >> 8;
			buf[8] = (uint8_t) frame.stream;

			if (frame.payload.size() != 0)
				buf.insert(buf.end(), frame.payload.begin(), frame.payload.end());

			return ConnectionInfo->Write(buf.data(), buf.size());
		}

		H2::Frame HTTP2Connection::ReadFrame() {
			char buf[9];

			if (!ConnectionInfo->Read(buf, 9))
				throw H2::Exception(HTTP2Error::FAILED_READ, "Failed to ReadFrame() [Fields]");

			H2::Frame frame;
			frame.length = ((buf[0] & 0xFF) << 16) | ((buf[1] & 0xFF)  << 8) | (buf[2] & 0xFF);

			if (frame.length > localSettings.maxFrameSize) {
				std::stringstream info("Failed to ReadFrame(): Frame size ");
				info << frame.length << " is larger than the localSettings' max size: " << localSettings.maxFrameSize;

				throw H2::Exception(HTTP2Error::FRAME_TOO_LARGE, info.str());
			}

			frame.payload.resize(frame.length);

			frame.type = buf[3];
			frame.flags = buf[4];
			frame.stream = ((buf[5] & 0xFF) << 24) | ((buf[6] & 0xFF) << 16) | ((buf[7] & 0xFF) << 8) | (buf[8] & 0xFF);

			if (!ConnectionInfo->Read(frame.payload.data(), frame.length))
				throw H2::Exception(HTTP2Error::FAILED_READ, "Failed to ReadFrame() [Payload]");

			return frame;
		}

		void HTTP2Connection::HandleFrameSettings(H2::Frame frame) {
			size_t i;
			uint16_t identifier;
			uint32_t value;
			const char *buf;

			if (frame.flags & H2::FrameFlags::ACK) {
				if (frame.length != 0)
					throw H2::Exception(HTTP2Error::SETTINGS_ACK_FLAG_INVALID_SIZE, "Received a SETTINGS frame with ACK flag set but length != 0");

				return;
			}

			if (frame.length % 6 != 0) {
				std::stringstream info;
				info << "Invalid SETTINGS frame length: " << frame.length << " (it should be a multiple of six)";
				throw H2::Exception(HTTP2Error::SETTINGS_INVALID_SIZE, info.str());
			}

			for (i = 0; i < frame.length / 6; i++) {
				buf = frame.payload.data() + i * 6;

				identifier = ((buf[0] & 0xFF) << 8) | (buf[1] & 0xFF);
				value = ((buf[2] & 0xFF) << 24) | ((buf[3] & 0xFF) << 16) | ((buf[4] & 0xFF) << 8) | (buf[5] & 0xFF);

				switch (identifier) {
					case H2::Settings::HEADER_TABLE_SIZE:
						remoteSettings.headerTableSize = value;
						break;
					case H2::Settings::ENABLE_PUSH:
						remoteSettings.enablePush = value;
						break;
					case H2::Settings::MAX_CONCURRENT_STREAMS:
						remoteSettings.maxConcurrentStreams = value;
						break;
					case H2::Settings::INITIAL_WINDOW_SIZE:
						remoteSettings.initialWindowSize = value;
						break;
					case H2::Settings::MAX_FRAME_SIZE:
						remoteSettings.maxFrameSize = value;
						break;
					case H2::Settings::MAX_HEADER_LIST_SIZE:
						remoteSettings.maxHeaderListSize = value;
						break;
					default: {
						std::stringstream info;
						info << "Unknown setting identifier: " << identifier << " with value: " << value;
						Logger::Debug(__PRETTY_FUNCTION__, info.str());
						break;
					}
				}
			}

			H2::Frame ack;
			ack.length = 0;
			ack.type = H2::FrameType::SETTINGS;
			ack.flags = 0;
			ack.stream = 0;
			SendFrame(ack);
		}

		void HTTP2Connection::HandleFrameGoaway(H2::Frame frame) {
			size_t i;
			uint32_t lastStream;
			uint32_t errorCode;

			lastStream = ((frame.payload[0] & 0xFF) << 24) | ((frame.payload[1] & 0xFF) << 16) | ((frame.payload[2] & 0xFF) << 8) | (frame.payload[3] & 0xFF);
			errorCode = ((frame.payload[4] & 0xFF) << 24) | ((frame.payload[5] & 0xFF) << 16) | ((frame.payload[6] & 0xFF) << 8) | (frame.payload[7] & 0xFF);

			std::stringstream info;
			info << "GOAWAY Information: LastStream: " << lastStream << " ErrorCode: " << errorCode;
			Logger::Warning(__PRETTY_FUNCTION__, info.str());

			if (frame.length > 8)
				for (i = 8; i < frame.length; i++)
					printf(" > 0x%hhX (%c)\n", frame.payload[i], frame.payload[i]);
		}
	}
}
