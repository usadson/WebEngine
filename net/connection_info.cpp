/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include "connection_info.hpp"

#include <chrono>
#include <iostream>
#include <sstream>
#include <vector>

#include <arpa/inet.h>
#include <cstring>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "ccompat.hpp"
#include "logger.hpp"

inline int
FUNC_SOCKET(int domain, int type, int protocol) noexcept {
	return socket(domain, type, protocol);
}

namespace Net {

	ConnectionInfo::~ConnectionInfo() {
		if (isAuthenticated)
			TLSDestroy();

		if (this->socket != 0) {
			if (close(this->socket) == -1) {
				int errorCode = errno;
				std::stringstream information;
				information << "Error on close() socket: "
							<< CCompat::GetErrnoName(errorCode)
							<< " (" << errorCode << ")";
				Logger::Warning("Net::ConnectionInfo::~", information.str());
			}
			this->socket = 0;
		}
	}

	std::string
	FormatTime(std::chrono::duration<int32_t, std::nano> nanoDuration) {
		std::stringstream durationStream;

		if (nanoDuration.count() > 1e6) {
			auto milliDuration = std::chrono::duration_cast<std::chrono::milliseconds>(nanoDuration);
			durationStream << milliDuration.count() << " ms";
		} else if (nanoDuration.count() > 1e3) {
			auto microDuration = std::chrono::duration_cast<std::chrono::microseconds>(nanoDuration);
			durationStream << microDuration.count() << " μs";
		} else
			durationStream << nanoDuration.count() << " ns";

		return durationStream.str();
	}

	bool
	ConnectionInfo::ResolveHostName() {
		int errorCode;
		struct addrinfo *address;
		struct addrinfo *result;

		this->socket = FUNC_SOCKET(AF_INET, SOCK_STREAM, 0);
		if (this->socket == -1) {
			errorCode = errno;
			std::stringstream information;
			information << "Error on socket(): " << CCompat::GetErrnoName(errorCode) << " (" << errorCode << ")";
			Logger::Warning("Net::ConnectionInfo::ResolveHostName", information.str());
			return false;
		}

		auto startResolve = std::chrono::high_resolution_clock::now();
		errorCode = getaddrinfo(hostName.c_str(), nullptr, nullptr, &result);
		timingDNS = FormatTime(std::chrono::high_resolution_clock::now() - startResolve);

		if (errorCode != 0) {
			std::stringstream information;
			information << "Error on getaddrinfo(): " << CCompat::GetErrnoName(errorCode) << " (" << errorCode << ")";
			Logger::Warning("Net::ConnectionInfo::ResolveHostName", information.str());
			freeaddrinfo(result);
			return false;
		}

		auto startPing = std::chrono::high_resolution_clock::now();
		for (address = result; address != nullptr; address = address->ai_next) {
			struct sockaddr_in connectAddress;
			connectAddress.sin_addr.s_addr = ((struct sockaddr_in *) address->ai_addr)->sin_addr.s_addr;
			connectAddress.sin_family = AF_INET;
			connectAddress.sin_port = htons(port);

			auto startConnect = std::chrono::high_resolution_clock::now();
			if (connect(this->socket, (struct sockaddr*) &connectAddress, sizeof(struct sockaddr_in)) != -1) {
				timingConnect = FormatTime(std::chrono::high_resolution_clock::now() - startConnect);
				timingPing = FormatTime(startConnect - startPing);
				freeaddrinfo(result);
				return true;
			}
		}

		timingPing = FormatTime(std::chrono::high_resolution_clock::now() - startPing);
		freeaddrinfo(result);
		close(this->socket);

		this->socket = 0;
		timingConnect = "0 ms (failed)";
		timingDNS = FormatTime(std::chrono::high_resolution_clock::now() - startResolve);

		return false;
	}

	bool
	ConnectionInfo::Connect() {
		if (this->socket != 0) {
			Logger::Warning("Net::ConnectionInfo::Connect", "Connect already called!");
			return false;
		}

		if (!ResolveHostName()) {
			Logger::Warning("Net::ConnectionInfo::Connect", "Failed to connect!");
			return false;
		}

		connected = true;

		if (secure) {
			auto startTLS = std::chrono::high_resolution_clock::now();
			isAuthenticated = TLSSetup();
			timingTLS = FormatTime(std::chrono::high_resolution_clock::now() - startTLS);

			if (!isAuthenticated) {
				timingTLS += " (failed)";
				Logger::Warning("Net::ConnectionInfo::Connect", "Failed to setup TLS!");
				return false;
			}
		} else {
			timingTLS = "";
		}

		Logger::Debug("Net::ConnectionInfo::Connect", "Timings:\n"
					  "\tDNS Resolving:             " + timingDNS + "\n"
					  "\tPinging other hosts:       " + timingPing + "\n"
					  "\tConnection Establishment:  " + timingConnect + "\n"
					  "\tTLS Handshake:             " + timingTLS);
		return true;
	}

	bool
	ConnectionInfo::Write(const char *buf, size_t len) {
		if (secure)
			return TLSWrite(buf, len);

		do {
			ssize_t ret = write(this->socket, buf, len);

			if (ret == -1)
				return false;

			buf += ret;
			len -= ret;
		} while (len > 0);

		return true;
	}

	std::optional<char>
	ConnectionInfo::ReadChar() {
		if (secure)
			return TLSReadChar();

		char character;
		if (read(this->socket, &character, 1) == -1)
			return std::optional<char>();

		return std::optional<char>(character);
	}

	bool
	ConnectionInfo::Read(char *buf, size_t len) {
		if (secure)
			return TLSRead(buf, len);

		while (len > 0) {
			ssize_t ret = read(this->socket, buf, len);

			if (ret == -1)
				return false;

			buf += ret;
			len -= ret;
		}

		return true;
	}

}
