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
		std::size_t position { 0 };

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
			if (inputBuffer.size() < position + 1) {
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
		HTTPConnection connection;

		HTTPConnectionTest() : connectionInfo(), dummyResponseInfo(), connection(connectionInfo) {
			connection.response = &dummyResponseInfo;
		}
	};

	TEST_F(HTTPConnectionTest, ConsumeSingleSpace) {
		connectionInfo.SetInputBuffer({});
		ASSERT_EQ(connection.ConsumeSingleSpace(), Net::HTTP::HTTPConnectionError::FAILED_READ_GENERIC);
		connectionInfo.SetInputBuffer({ ' ' });
		ASSERT_EQ(connection.ConsumeSingleSpace(), Net::HTTP::HTTPConnectionError::NO_ERROR);
		for (uint16_t i = 0; i < 255; i++) {
			connectionInfo.SetInputBuffer({ static_cast<char>(i) });
			if (i == 0x20)
				ASSERT_EQ(connection.ConsumeSingleSpace(), Net::HTTP::HTTPConnectionError::NO_ERROR);
			else
				ASSERT_EQ(connection.ConsumeSingleSpace(), Net::HTTP::HTTPConnectionError::WHITESPACE_EXPECTED);
		}
	}

	TEST_F(HTTPConnectionTest, ConsumeReasonPhrase) {
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

		std::vector<std::vector<char>> invalidInputs = { { 'A', '0', '0' },
			{ '2', 'A', '0' },
			{ '2', '0', 'A' },
			{ '2', 'A', 'A' },
			{ 'A', 'A', '0' },
			{ 'A', 'A', 'A' },
			{ '0', '0', '0' },
			{ '0', '0', '1' },
			{ '0', '1', '0' },
			{ '6', '0', '0' },
			{ '7', '9', '9' },
			{ '8', '2', '6' } };

		Logger::SetOutputState(false);

		for (size_t i = 0; i < invalidInputs.size(); i++) {
			connectionInfo.SetInputBuffer(invalidInputs[i]);
			ASSERT_EQ(connection.ConsumeStatusCode(), Net::HTTP::HTTPConnectionError::INCORRECT_STATUS_CODE)
				<< "invalidInputs[" << i << "]";
		}

		Logger::SetOutputState(true);
	}

	TEST_F(HTTPConnectionTest, ConsumeHTTPVersion) {
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

		std::vector<std::vector<char>> invalidInputs = { { 'A', 'T', 'T', 'P', '/', '1', '.', '1' },
			{ 'H', 'A', 'T', 'P', '/', '1', '.', '1' },
			{ 'H', 'T', 'A', 'P', '/', '1', '.', '1' },
			{ 'H', 'T', 'T', 'A', '/', '1', '.', '1' },
			{ 'H', 'T', 'T', 'P', 'A', '1', '.', '1' },
			{ 'H', 'T', 'T', 'P', 'P', 'A', '.', '1' },
			{ 'H', 'T', 'T', 'P', '/', '1', 'A', '1' },
			{ 'H', 'T', 'T', 'P', '/', '1', '.', 'A' },
			{ 0x00, 'T', 'T', 'P', '/', '1', '.', '1' },
			{ 'H', 0x00, 'T', 'P', '/', '1', '.', '1' },
			{ 'H', 'T', 0x00, 'P', '/', '1', '.', '1' },
			{ 'H', 'T', 'T', 0x00, '/', '1', '.', '1' },
			{ 'H', 'T', 'T', 'P', 0x00, '1', '.', '1' },
			{ 'H', 'T', 'T', 'P', '/', 0x00, '.', '1' },
			{ 'H', 'T', 'T', 'P', '/', '1', 0x00, '1' },
			{ 'H', 'T', 'T', 'P', '/', '1', '.', 0x00 } };

		for (size_t i = 0; i < invalidInputs.size(); i++) {
			connectionInfo.SetInputBuffer(invalidInputs[i]);
			ASSERT_EQ(connection.ConsumeHTTPVersion(), Net::HTTP::HTTPConnectionError::INCORRECT_PROTOCOL)
				<< "invalidInputs[" << i << "]";
		}

		Logger::SetOutputState(true);
	}

} // namespace Net::HTTP

int
main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
