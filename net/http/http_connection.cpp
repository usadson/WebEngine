/**
 * Copyright (C) 2020 Tristan
 *
 * All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/* TODO Implement vector max-lengths? */

#include "http_connection.hpp"

#include <iostream>
#include <sstream>
#include <vector>

#include <cstring>

#include "logger.hpp"

namespace Net {
	namespace HTTP {
		std::map<HTTPConnectionError, std::string> httpConnectionErrorNames = {
			{ HTTPConnectionError::FAILED_READ_GENERIC, "FAILED_READ_GENERIC" },
			{ HTTPConnectionError::FAILED_READ_HEADER_FIELD_GENERIC, "FAILED_READ_HEADER_FIELD_GENERIC" },
			{ HTTPConnectionError::FAILED_READ_HEADER_FIELD_NAME, "FAILED_READ_HEADER_FIELD_NAME" },
			{ HTTPConnectionError::FAILED_READ_HTTP_VERSION, "FAILED_READ_HTTP_VERSION" },
			{ HTTPConnectionError::FAILED_READ_HEADER_FIELD_VALUE, "FAILED_READ_HEADER_FIELD_VALUE" },
			{ HTTPConnectionError::FAILED_READ_REASON_PHRASE, "FAILED_READ_REASON_PHRASE" },
			{ HTTPConnectionError::FAILED_READ_STATUS_CODE, "FAILED_READ_STATUS_CODE" },
			{ HTTPConnectionError::FAILED_READ_MESSAGE_BODY, "FAILED_READ_MESSAGE_BODY" },
			{ HTTPConnectionError::FAILED_WRITE_REQUEST, "FAILED_WRITE_REQUEST" },
			{ HTTPConnectionError::INCORRECT_HEADER_FIELD_GENERIC, "INCORRECT_HEADER_FIELD_GENERIC" },
			{ HTTPConnectionError::INCORRECT_HEADER_FIELD_NAME, "INCORRECT_HEADER_FIELD_NAME" },
			{ HTTPConnectionError::INCORRECT_HEADER_FIELD_VALUE, "INCORRECT_HEADER_FIELD_VALUE" },
			{ HTTPConnectionError::INCORRECT_REASON_PHRASE, "INCORRECT_REASON_PHRASE" },
			{ HTTPConnectionError::INCORRECT_START_LINE, "INCORRECT_START_LINE" },
			{ HTTPConnectionError::NO_ERROR, "NO_ERROR" },
			{ HTTPConnectionError::NOT_CONNECTED, "NOT_CONNECTED" },
		};

		HTTPConnection::HTTPConnection(const Net::ConnectionInfo &inConnectionInfo)
				: connectionInfo(inConnectionInfo) {
			if (!connectionInfo.Connect()) {
				std::stringstream information;
				information << "Failed to connect! Host: \"" << connectionInfo.hostName << "\":" << connectionInfo.port;
				Logger::Error("HTTPConnection", information.str());
				return;
			}
		}

		HTTPConnection::~HTTPConnection() {
		}

		HTTPConnectionError
		HTTPConnection::ConsumeHTTPVersion(HTTPResponseInfo *response) {
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
			if (protocolData[5] != '1' || protocolData[7] != '1')
				Logger::Warning("HTTPConnection::ConsumeVersion", "HTTP Version isn't \"HTTP/1.1\": \"" + response->httpVersion + "\".");

			return HTTPConnectionError::NO_ERROR;
		}

		HTTPConnectionError
		HTTPConnection::ConsumeStatusCode(HTTPResponseInfo *response) {
			/* Read status code */
			std::vector<char> statusCode(4);
			size_t i;

			if (!connectionInfo.Read(statusCode.data(), 3))
				return HTTPConnectionError::FAILED_READ_STATUS_CODE;
			statusCode.push_back('\0');

			/* Validate HTTP Version */
			bool parsableInteger = true;
			for (i = 0; i < 3; i++) {
				if (statusCode[i] < 0x30 || statusCode[i] > 0x39) {
					// Treat as a '400 Bad Request' status code (right?)
					Logger::Warning("HTTPConnection::ConsumeStatusCode", std::string("Incorrect status-code (should be a digit): ") + statusCode.data());
					parsableInteger = false;
					break;
				}
			}

			if (parsableInteger) {
				if (statusCode[0] < 0x31 || statusCode[0] > 0x35)
					Logger::Warning("HTTPConnection::ConsumeStatusCode", std::string("Incorrect status-code (uncategorized): ") + statusCode.data());

				response->statusCode = (statusCode[0] - 0x30) * 100
									 + (statusCode[1] - 0x30) * 10
									 + (statusCode[2] - 0x30);
			}

			return HTTPConnectionError::NO_ERROR;
		}

		/**
		 * Note: a reason phrase may be empty.
		 */
		HTTPConnectionError
		HTTPConnection::ConsumeReasonPhrase(HTTPResponseInfo *response) {
			std::vector<char> reasonPhrase;
			std::optional<char> character;
			while ((character = connectionInfo.ReadChar()).has_value()) {
				if (character == '\r') {
					reasonPhrase.push_back('\0');
					response->reasonPhrase = reasonPhrase.data();
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

				reasonPhrase.push_back('\0');
				response->reasonPhrase = reasonPhrase.data();
				return HTTPConnectionError::INCORRECT_REASON_PHRASE;
			}

			reasonPhrase.push_back('\0');
			response->reasonPhrase = reasonPhrase.data();
			return HTTPConnectionError::FAILED_READ_REASON_PHRASE;
		}

		HTTPConnectionError
		HTTPConnection::ConsumeHeaderField(HTTPResponseInfo *response, char firstCharacter) {
			std::vector<char> fieldName;
			std::vector<char> fieldValue;
			std::optional<char> character;
			char *nullCharacterPosition;

			/* Consume field-name */
			fieldName.push_back(firstCharacter);
			bool endName = false;
			while (!endName) {
				character = connectionInfo.ReadChar();

				if (!character.has_value())
					return HTTPConnectionError::FAILED_READ_HEADER_FIELD_NAME;

				/*
				 * field-name = token
				 * token = 1*tchar
				 * tchar = "!" / "#" / "$" / "%" / "&" / "'" / "*" / "+" / "-"
				 * / "." / "^" / "_" / "`" / "|" / "~" / DIGIT / ALPHA
				 */
				switch (character.value()) {
					case ':':
						endName = true;
						break;
					case '!':
					case '#':
					case '$':
					case '%':
					case '&':
					case '\'':
					case '*':
					case '+':
					case '-':
					case '.':
					case '^':
					case '_':
					case '`':
					case '|':
					case '~':
						fieldName.push_back(character.value());
						break;
					default:
						if ((character.value() >= 0x30 && character.value() <= 0x39) || // DIGIT
							(character.value() >= 0x41 && character.value() <= 0x5A) || // ALPHA (UPPER)
							(character.value() >= 0x61 && character.value() <= 0x7A)) { // ALPHA (LOWER)
							fieldName.push_back(character.value());
						} else {
							return HTTPConnectionError::INCORRECT_HEADER_FIELD_NAME;
						}
						break;
				}
			}

			/* Consume OWS (Optional Whitespaces) */
			while (true) {
				character = connectionInfo.ReadChar();

				if (!character.has_value())
					return HTTPConnectionError::FAILED_READ_HEADER_FIELD_GENERIC;

				if (character != ' ' && character != '\t')
					break;
			}

			/* Consume header-value */
			/* obs-fold (optional line folding) isn't supported. */
			do {
				if (character == '\r') {
					character = connectionInfo.ReadChar();
					if (!character.has_value())
						return HTTPConnectionError::FAILED_READ_HEADER_FIELD_GENERIC;
					if (character != '\n')
						return HTTPConnectionError::INCORRECT_HEADER_FIELD_GENERIC;
					break;
				}

				if ((character >= 0x21 && character <= 0x7E) || // VCHAR
					(character >= 0x80 && character <= 0xFF) || // obs-text
					 character == ' '  ||						// SP
					 character == '\t')							// HTAB
					fieldValue.push_back(character.value());
				else
					return HTTPConnectionError::INCORRECT_HEADER_FIELD_VALUE;

				/* Set next character */
				character = connectionInfo.ReadChar();

				if (!character.has_value())
					return HTTPConnectionError::FAILED_READ_HEADER_FIELD_VALUE;
			} while (true);

			/* Store in strings */
			fieldName.push_back('\0');
			fieldValue.push_back('\0');

			/* Trim end of OWS's. */
			char *fieldValueString = fieldValue.data();
			char *lastSpace = (char *) strrchr(fieldValueString, ' ');
			char *lastHTab = (char *) strrchr(fieldValueString, '\t');

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
		HTTPConnection::Request(HTTPResponseInfo *response, const std::string &method, const std::string &path) {
			if (!connectionInfo.connected ||
				(connectionInfo.secure && !connectionInfo.isAuthenticated)) {
				return HTTPConnectionError::NOT_CONNECTED;
			}

			std::optional<char> singleCharacter;
			std::stringstream request;
			HTTPConnectionError subroutineError;

			// TODO A stringstream isn't really needed at this point, so should
			// we use a vector for performance reasons?
			request << method << ' ' << path << " HTTP/1.1\r\n";
			request << "Host: " << connectionInfo.hostName << "\r\n";
			request << "TE: Trailers\r\n";
			request << "\r\n";

			std::string str = request.str();
			if (!connectionInfo.Write(str.c_str(), str.length()))
				return HTTPConnectionError::FAILED_WRITE_REQUEST;

			/* Consume HTTP-Version */
			subroutineError = ConsumeHTTPVersion(response);
			if (subroutineError != HTTPConnectionError::NO_ERROR)
				return subroutineError;

			/* Consume space between 'HTTP-version' and 'status-code' */
			singleCharacter = connectionInfo.ReadChar();
			if (!singleCharacter.has_value())
				return HTTPConnectionError::FAILED_READ_GENERIC;

			if (singleCharacter.value() != ' ')
				return HTTPConnectionError::INCORRECT_START_LINE;

			/* Consume 'status-code' */
			subroutineError = ConsumeStatusCode(response);
			if (subroutineError != HTTPConnectionError::NO_ERROR)
				return subroutineError;

			/* Consume space between 'status-code' and 'reason-phrase' */
			singleCharacter = connectionInfo.ReadChar();
			if (!singleCharacter.has_value())
				return HTTPConnectionError::FAILED_READ_GENERIC;

			if (singleCharacter.value() != ' ')
				return HTTPConnectionError::INCORRECT_START_LINE;

			/* Consume 'reason-phrase' */
			subroutineError = ConsumeReasonPhrase(response);
			if (subroutineError != HTTPConnectionError::NO_ERROR)
				return subroutineError;

			/* Consume new-line after 'reason-phrase', the carriage-return is
			 * already consumed by ConsumeReasonPhrase. */
			singleCharacter = connectionInfo.ReadChar();
			if (!singleCharacter.has_value())
				return HTTPConnectionError::FAILED_READ_GENERIC;

			if (singleCharacter.value() != '\n')
				return HTTPConnectionError::INCORRECT_START_LINE;

			do {
				singleCharacter = connectionInfo.ReadChar();
				if (!singleCharacter.has_value())
					return HTTPConnectionError::FAILED_READ_GENERIC;

				if (singleCharacter.value() == '\r') {
					singleCharacter = connectionInfo.ReadChar();
					if (!singleCharacter.has_value() || singleCharacter != '\n')
						Logger::Warning("HTTPConnection::Request", "Incorrect CRLF");
					break;
				}

				subroutineError = ConsumeHeaderField(response, singleCharacter.value());

				if (subroutineError != HTTPConnectionError::NO_ERROR)
					return subroutineError;
			} while (true);

			/* Consume message-body */
			std::optional<size_t> contentLength = response->GetHeaderUnsigned("content-length");
			if (contentLength.has_value()) {
				/* Make space in HTTPResponseInfo::MessageBody */
				response->messageBody.resize(contentLength.value());

				if (!connectionInfo.Read(response->messageBody.data(), contentLength.value())) {
					return HTTPConnectionError::FAILED_READ_MESSAGE_BODY;
				}
			}

			return HTTPConnectionError::NO_ERROR;
		}

		HTTPConnectionError
		HTTPConnection::RequestNavigation(HTTPResponseInfo *response, const std::string &path) {
			return Request(response, "GET", path);
		}
	}
}
