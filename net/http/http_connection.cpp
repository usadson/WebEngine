/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

/* TODO Implement vector max-lengths? */

#include "http_connection.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

#include <cstring>

#include "logger.hpp"

namespace Net::HTTP {
	HTTPConnection::HTTPConnection(Net::ConnectionInfo &inConnectionInfo) : connectionInfo(inConnectionInfo) {
		if (!connectionInfo.Connect()) {
			std::stringstream information;
			information << "Failed to connect! Host: \"" << connectionInfo.hostName << "\":" << connectionInfo.port;
			Logger::Error("HTTPConnection", information.str());
			return;
		}
	}

	HTTPConnectionError
	HTTPConnection::ConsumeHTTPVersion() {
		/* Read protocol version (= HTTP-version) */
		std::vector<char> protocolData(9);
		if (!connectionInfo.Read(protocolData.data(), 8))
			return HTTPConnectionError::FAILED_READ_HTTP_VERSION;

		/* Validate HTTP Version */
		if (memcmp(protocolData.data(), "HTTP/", 5) != 0 || protocolData[6] != '.')
			return HTTPConnectionError::INCORRECT_PROTOCOL;

		/* Store HTTP Version */
		protocolData.push_back('\0');
		response->httpVersion = std::string(protocolData.data());

		/* Check if version is 'HTTP/1.1' */
		if (protocolData[5] != '1' || protocolData[7] != '1') {
			if (protocolData[5] < '0' || protocolData[5] > '9' || protocolData[7] < '0' || protocolData[7] > '9') {
				return HTTPConnectionError::INCORRECT_PROTOCOL;
			}
			Logger::Warning("HTTPConnection::ConsumeVersion",
				"HTTP Version isn't \"HTTP/1.1\": \"" + response->httpVersion + "\".");
		}

		return HTTPConnectionError::NO_ERROR;
	}

	HTTPConnectionError
	HTTPConnection::ConsumeStatusCode() {
		/* Read status code */
		std::vector<char> statusCode(3);

		if (!connectionInfo.Read(statusCode.data(), 3))
			return HTTPConnectionError::FAILED_READ_STATUS_CODE;

		/* Validate status code */
		if ((statusCode[0] < '1' || statusCode[0] > '5') || (statusCode[1] < '0' || statusCode[1] > '9')
			|| (statusCode[2] < '0' || statusCode[2] > '9')) {
			std::stringstream info;
			info << std::hex << "Incorrect status-code: " << static_cast<uint16_t>(statusCode.at(0))
				 << static_cast<uint16_t>(statusCode.at(1)) << static_cast<uint16_t>(statusCode.at(2)) << std::dec;
			Logger::Warning("HTTPConnection::ConsumeStatusCode", info.str());
			return HTTPConnectionError::INCORRECT_STATUS_CODE;
		}

		response->statusCode = (statusCode[0] - 0x30) * 100 + (statusCode[1] - 0x30) * 10 + (statusCode[2] - 0x30);

		return HTTPConnectionError::NO_ERROR;
	}

	/**
	 * Note: a reason phrase may be empty.
	 */
	HTTPConnectionError
	HTTPConnection::ConsumeReasonPhrase() {
		std::vector<char> reasonPhrase;
		std::optional<char> character;
		while ((character = connectionInfo.ReadChar()).has_value()) {
			if (character == '\r') {
				response->reasonPhrase = std::string(std::begin(reasonPhrase), std::end(reasonPhrase));
				return HTTPConnectionError::NO_ERROR;
			}

			// HTAB	    = 0x09 = '\t'
			// SP       = 0x20 = ' '
			// VCHAR    = 0x21 - 0x7E (visible character)
			// obs-text = 0x80 - 0xFF
			if (character == '\t' || // HTAB
				character >= 0x20) {
				reasonPhrase.push_back(character.value());
				continue;
			}

			return HTTPConnectionError::INCORRECT_REASON_PHRASE;
		}

		return HTTPConnectionError::FAILED_READ_REASON_PHRASE;
	}

	HTTPConnectionError
	HTTPConnection::ConsumeHeaderField(char firstCharacter) {
		std::vector<char> fieldName;
		std::vector<char> fieldValue;
		char *nullCharacterPosition;
		HTTPConnectionError subroutineError;

		/* Consume field-name */
		fieldName.push_back(firstCharacter);
		subroutineError = ConsumeHeaderFieldName(&fieldName);
		if (subroutineError != HTTPConnectionError::NO_ERROR)
			return subroutineError;

		/* Consume OWS (Optional Whitespaces) */
		while (true) {
			std::optional<char> character = connectionInfo.ReadChar();

			if (!character.has_value())
				return HTTPConnectionError::FAILED_READ_HEADER_FIELD_GENERIC;

			if (character != ' ' && character != '\t')
				break;
		}

		/* Consume header-value */
		subroutineError = ConsumeHeaderFieldValue(&fieldValue);
		if (subroutineError != HTTPConnectionError::NO_ERROR)
			return subroutineError;

		/* Store in strings */
		fieldName.push_back('\0');
		fieldValue.push_back('\0');

		/* Trim end of OWS's. */
		char *fieldValueString = fieldValue.data();
		char *lastSpace = strrchr(fieldValueString, ' ');
		char *lastHTab = strrchr(fieldValueString, '\t');

		if (lastSpace != nullptr)
			if (lastHTab != nullptr)
				if (lastHTab > lastSpace)
					nullCharacterPosition = lastSpace;
				else
					nullCharacterPosition = lastHTab;
			else
				nullCharacterPosition = lastSpace;
		else if (lastHTab != nullptr)
			nullCharacterPosition = lastHTab;
		else
			nullCharacterPosition = fieldValueString + fieldValue.size() - 1;
		*nullCharacterPosition = 0;

		response->headers.push_back({ std::string(fieldName.data()), std::string(fieldValueString) });
		return HTTPConnectionError::NO_ERROR;
	}

	HTTPConnectionError
	HTTPConnection::ConsumeHeaderFieldValue(std::vector<char> *dest) {
		/* obs-fold (optional line folding) isn't supported. */
		std::optional<char> character;
		while (true) {
			/* Set next character */
			character = connectionInfo.ReadChar();

			if (!character.has_value())
				return HTTPConnectionError::FAILED_READ_HEADER_FIELD_VALUE;

			if (character == '\r') {
				character = connectionInfo.ReadChar();
				if (!character.has_value())
					return HTTPConnectionError::FAILED_READ_HEADER_FIELD_GENERIC;
				if (character != '\n')
					return HTTPConnectionError::INCORRECT_HEADER_FIELD_GENERIC;
				return HTTPConnectionError::NO_ERROR;
			}

			if ((character >= 0x21 && character <= 0x7E) || // VCHAR
				(character >= 0x80 && character <= 0xFF) || // obs-text
				character == ' ' || character == '\t') {	// SP / HTAB
				dest->push_back(character.value());
			} else {
				return HTTPConnectionError::INCORRECT_HEADER_FIELD_VALUE;
			}
		}
	}

	HTTPConnectionError
	HTTPConnection::ConsumeHeaderFieldName(std::vector<char> *dest) {
		static const std::vector<char> unreservedCharacters
			= { '!', '#', '$', '%', '&', '\'', '*', '+', '-', '.', '^', '_', '`', '|', '~' };

		std::optional<char> character;

		while (true) {
			character = connectionInfo.ReadChar();

			if (!character.has_value())
				return HTTPConnectionError::FAILED_READ_HEADER_FIELD_NAME;

			if (character.value() == ':')
				return HTTPConnectionError::NO_ERROR;

			if (std::find(std::begin(unreservedCharacters),
					std::end(unreservedCharacters),
					character.value())
					!= std::end(unreservedCharacters)
				|| (character.value() >= 0x30 && character.value() <= 0x39) || // DIGIT
				(character.value() >= 0x41 && character.value() <= 0x5A) ||	   // ALPHA (UPPER)
				(character.value() >= 0x61 && character.value() <= 0x7A)) {
				dest->push_back(character.value());
			} else /* Invalid character */
				return HTTPConnectionError::INCORRECT_HEADER_FIELD_NAME;
		}
	}

	HTTPConnectionError
	HTTPConnection::ConsumeSingleSpace() {
		auto singleCharacter = connectionInfo.ReadChar();
		if (!singleCharacter.has_value())
			return HTTPConnectionError::FAILED_READ_GENERIC;

		if (singleCharacter.value() != ' ')
			return HTTPConnectionError::WHITESPACE_EXPECTED;

		return HTTPConnectionError::NO_ERROR;
	}

	HTTPConnectionError
	HTTPConnection::ConsumeHeaders() {
		do {
			auto singleCharacter = connectionInfo.ReadChar();
			if (!singleCharacter.has_value())
				return HTTPConnectionError::FAILED_READ_GENERIC;

			if (singleCharacter.value() == '\r') {
				singleCharacter = connectionInfo.ReadChar();
				if (!singleCharacter.has_value() || singleCharacter != '\n')
					Logger::Warning("HTTPConnection::Request", "Incorrect CRLF");
				break;
			}

			auto error = ConsumeHeaderField(singleCharacter.value());
			if (error != HTTPConnectionError::NO_ERROR)
				return error;
		} while (true);

		return HTTPConnectionError::NO_ERROR;
	}

	HTTPConnectionError
	HTTPConnection::ConsumeMessageBody() {
		std::optional<std::size_t> contentLength = response->GetHeaderUnsigned("content-length");
		if (contentLength.has_value()) {
			/* Make space in HTTPResponseInfo::MessageBody */
			response->messageBody.resize(contentLength.value());

			if (!connectionInfo.Read(response->messageBody.data(), contentLength.value())) {
				return HTTPConnectionError::FAILED_READ_MESSAGE_BODY;
			}
		} else if (response->GetHeader("content-length")) {
			return HTTPConnectionError::INVALID_CONTENT_LENGTH;
		}

		return HTTPConnectionError::NO_ERROR;
	}

	HTTPConnectionError
	HTTPConnection::ConsumeNewLine() {
		auto singleCharacter = connectionInfo.ReadChar();
		if (!singleCharacter.has_value())
			return HTTPConnectionError::FAILED_READ_GENERIC;
		if (singleCharacter.value() != '\n')
			return HTTPConnectionError::NEWLINE_EXPECTED;
		return HTTPConnectionError::NO_ERROR;
	}

	HTTPConnectionError
	HTTPConnection::Request(HTTPResponseInfo *response, const std::string &method, const std::string &path) {
		if (!connectionInfo.connected || (connectionInfo.secure && !connectionInfo.isAuthenticated)) {
			return HTTPConnectionError::NOT_CONNECTED;
		}

		this->response = response;

		// TODO A stringstream isn't really needed at this point, so should
		// we use a vector for performance reasons?
		std::stringstream request;
		request << method << ' ' << path << " HTTP/1.1\r\n";
		request << "Host: " << connectionInfo.hostName << "\r\n";
		request << "TE: Trailers\r\n";
		request << "\r\n";

		std::string str = request.str();
		if (!connectionInfo.Write(str.c_str(), str.length()))
			return HTTPConnectionError::FAILED_WRITE_REQUEST;

		for (const auto &subroutine : { &HTTPConnection::ConsumeHTTPVersion,
				 &HTTPConnection::ConsumeSingleSpace,
				 &HTTPConnection::ConsumeStatusCode,
				 &HTTPConnection::ConsumeSingleSpace,
				 &HTTPConnection::ConsumeReasonPhrase,
				 &HTTPConnection::ConsumeNewLine,
				 &HTTPConnection::ConsumeHeaders,
				 &HTTPConnection::ConsumeMessageBody }) {
			auto error = (this->*subroutine)();
			if (error != HTTPConnectionError::NO_ERROR)
				return error;
		}

		return HTTPConnectionError::NO_ERROR;
	}

	HTTPConnectionError
	HTTPConnection::RequestNavigation(HTTPResponseInfo *response, const std::string &path) {
		return Request(response, "GET", path);
	}
} // namespace Net::HTTP
