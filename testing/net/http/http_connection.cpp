/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <sstream>

#include <climits>

#include <gtest/gtest.h>

#include "logger.hpp"
#include "net/connection_info.hpp"

#define HTTP_CONNECTION_ENABLE_TESTING
#include "net/http/http_connection.hpp"

namespace Net {

	class BufferedConnectionInfo : public ConnectionInfo {
	  private:
		std::vector<char> inputBuffer;
		std::vector<char> outputBuffer;
		std::size_t position;

	  public:
		BufferedConnectionInfo() : ConnectionInfo("destination.test", 80) {
		}

		bool
		Connect() override {
			return true;
		}

		void
		SetInputBuffer(const std::vector<char> &input) {
			inputBuffer = input;
			position = 0;
		}

		bool
		Read(char *out, std::size_t length) override {
			if (inputBuffer.size() < position + length) {
				std::cerr << "Read() outside the buffer size! paramLength=" << length << " position=" << position
						  << " inputBuffer.size()=" << inputBuffer.size() << '\n';
				return false;
			}

			memcpy(out, inputBuffer.data() + position, length);
			return true;
		}

		std::optional<char>
		ReadChar() override {
			if (inputBuffer.size() < position) {
				std::cerr << "ReadChar() outside the buffer size! inputBuffer.size()=" << inputBuffer.size() << '\n';
				return {};
			}
			return { inputBuffer[position++] };
		}

		bool
		Write(const char *in, std::size_t length) override {
			outputBuffer.insert(std::begin(outputBuffer), in, in + length);
			return true;
		}
	};

} // namespace Net

namespace Net::HTTP {

	class HTTPConnectionTest : public ::testing::Test {
	  public:
		Net::BufferedConnectionInfo connectionInfo;
		HTTPResponseInfo dummyResponseInfo;
	};

	TEST_F(HTTPConnectionTest, ConsumeReasonPhrase) {
		HTTPConnection connection(connectionInfo);
		connection.response = &dummyResponseInfo;
		std::vector<char> input;

		std::vector<std::string> validInputs = { "\r", " \r", " ok \r", "agjia;h skghah aghaha;hj \r" };

		for (const auto &string : validInputs) {
			input.insert(std::begin(input), std::begin(string), std::end(string));
			connectionInfo.SetInputBuffer(input);
			ASSERT_EQ(connection.ConsumeReasonPhrase(), Net::HTTP::HTTPConnectionError::NO_ERROR)
				<< "String: '" << string << "\n' failed";
		}

		for (char i = 0; i < 20; i++) {
			if (i == '\t' || i == '\r')
				continue;
			input[0] = i;
			connectionInfo.SetInputBuffer(input);
			ASSERT_EQ(connection.ConsumeReasonPhrase(), Net::HTTP::HTTPConnectionError::INCORRECT_REASON_PHRASE)
				<< "Invalid: " << static_cast<uint16_t>(i);
		}
	}

	TEST_F(HTTPConnectionTest, ConsumeStatusCode) {
		HTTPConnection connection(connectionInfo);
		connection.response = &dummyResponseInfo;

		Logger::SetOutputState(false);

		// Valid Inputs
		std::vector<char> input = { '2', '0', '0' };
		for (char a = '1'; a <= '5'; a++) {
			input[0] = a;
			for (char b = '0'; b <= '9'; b++) {
				input[1] = b;
				for (char c = '0'; c <= '9'; c++) {
					input[2] = c;
					connectionInfo.SetInputBuffer(input);
					ASSERT_EQ(connection.ConsumeStatusCode(), Net::HTTP::HTTPConnectionError::NO_ERROR)
						<< "input: " << a << b << c;
				}
			}
		}

		std::vector<std::vector<char> > invalidInputs
			= { { 'A', '0', '0' }, { '2', 'A', '0' }, { '2', '0', 'A' }, { '2', 'A', 'A' },
				{ 'A', 'A', '0' }, { 'A', 'A', 'A' }, { '0', '0', '0' }, { '0', '0', '1' },
				{ '0', '1', '0' }, { '6', '0', '0' }, { '7', '9', '9' }, { '8', '2', '6' } };

		for (size_t i = 0; i < invalidInputs.size(); i++) {
			connectionInfo.SetInputBuffer(invalidInputs[i]);
			ASSERT_EQ(connection.ConsumeStatusCode(), Net::HTTP::HTTPConnectionError::INCORRECT_STATUS_CODE)
				<< "invalidInputs[" << i << "]";
		}
	}

	TEST_F(HTTPConnectionTest, ConsumeHTTPVersion) {
		HTTPConnection connection(connectionInfo);
		connection.response = &dummyResponseInfo;

		Logger::SetOutputState(false);

		// Valid Inputs
		std::vector<char> input = { 'H', 'T', 'T', 'P', '/', '0', '.', '0' };
		for (char major = '0'; major <= '9'; major++) {
			input[5] = major;
			for (char minor = '0'; minor <= '9'; minor++) {
				input[7] = minor;
				connectionInfo.SetInputBuffer(input);
				ASSERT_EQ(connection.ConsumeHTTPVersion(), Net::HTTP::HTTPConnectionError::NO_ERROR);
			}
		}

		std::vector<std::vector<char> > invalidInputs
			= { { 'A', 'T', 'T', 'P', '/', '1', '.', '1' },	 { 'H', 'A', 'T', 'P', '/', '1', '.', '1' },
				{ 'H', 'T', 'A', 'P', '/', '1', '.', '1' },	 { 'H', 'T', 'T', 'A', '/', '1', '.', '1' },
				{ 'H', 'T', 'T', 'P', 'A', '1', '.', '1' },	 { 'H', 'T', 'T', 'P', 'P', 'A', '.', '1' },
				{ 'H', 'T', 'T', 'P', '/', '1', 'A', '1' },	 { 'H', 'T', 'T', 'P', '/', '1', '.', 'A' },
				{ 0x00, 'T', 'T', 'P', '/', '1', '.', '1' }, { 'H', 0x00, 'T', 'P', '/', '1', '.', '1' },
				{ 'H', 'T', 0x00, 'P', '/', '1', '.', '1' }, { 'H', 'T', 'T', 0x00, '/', '1', '.', '1' },
				{ 'H', 'T', 'T', 'P', 0x00, '1', '.', '1' }, { 'H', 'T', 'T', 'P', '/', 0x00, '.', '1' },
				{ 'H', 'T', 'T', 'P', '/', '1', 0x00, '1' }, { 'H', 'T', 'T', 'P', '/', '1', '.', 0x00 } };

		for (size_t i = 0; i < invalidInputs.size(); i++) {
			connectionInfo.SetInputBuffer(invalidInputs[i]);
			ASSERT_EQ(connection.ConsumeHTTPVersion(), Net::HTTP::HTTPConnectionError::INCORRECT_PROTOCOL)
				<< "invalidInputs[" << i << "]";
		}

		/*
		 * An idea for a kind of fuzzer, but running it single-threadedly is impossible.
		std::vector<char> input(8);
		for (uint16_t a = 0; a <= 0xFF; a++) {
			input[0] = static_cast<char>(a);
			for (uint16_t b = 0; b <= 0xFF; b++) {
				input[1] = static_cast<char>(b);
				for (uint16_t c = 0; c <= 0xFF; c++) {
					input[2] = static_cast<char>(c);
					for (uint16_t d = 0; d <= 0xFF; d++) {
						input[3] = static_cast<char>(d);
						for (uint16_t e = 0; e <= 0xFF; e++) {
							input[4] = static_cast<char>(e);
							for (char major = '0'; major <= '9'; major++) {
								input[5] = major;
								for (uint16_t f = 0; f <= 0xFF; f++) {
									input[6] = static_cast<char>(f);
									for (char minor = '0'; minor <= '9'; minor++) {
										std::cout << std::hex << a << b << c << d << e << major << f << minor << '\r';z
										input[7] = minor;
										connectionInfo.SetInputBuffer(input);
										if (a == 'H' && b == 'T' && c == 'T' && d == 'P' && e == '/' && f == '.' &&
		major >= '0' && major >= '0' && major <= '9' && minor >= '0' && minor <= '9') {
											ASSERT_EQ(connection.ConsumeHTTPVersion(),
		Net::HTTP::HTTPConnectionError::NO_ERROR); } else { ASSERT_EQ(connection.ConsumeHTTPVersion(),
		Net::HTTP::HTTPConnectionError::INCORRECT_PROTOCOL);
										}
									}
								}
							}
						}
					}
				}
			}
		}
		*/

		Logger::SetOutputState(true);
	}

} // namespace Net::HTTP

int
main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
