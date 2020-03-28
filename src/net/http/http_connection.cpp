#include "http_connection.hpp" 

#include <iostream>
#include <sstream>
#include <vector>

#include <cstring>

#include "logger.hpp"

namespace Net {
	namespace HTTP {
		HTTPConnection::HTTPConnection(Net::ConnectionInfo connectionInfo)
				: ConnectionInfo(connectionInfo) {
			if (!ConnectionInfo.Connect()) {
				std::stringstream information;
				information << "Failed to connect! Host: \"" << connectionInfo.HostName << "\":" << connectionInfo.Port;
				Logger::Error("HTTPConnection", information.str());
				return;
			}
		}

		HTTPConnection::~HTTPConnection() {
			
		}
		
		HTTPConnectionError HTTPConnection::ConsumeHTTPVersion(HTTPResponseInfo *response) {
			/* Read protocol version (= HTTP-version) */
			std::vector<char> protocolData(9);
			if (!ConnectionInfo.Read(protocolData.data(), 8))
				return HTTPConnectionError::FAILED_READ_HTTP_VERSION;

			/* Validate HTTP Version */
			if (memcmp(protocolData.data(), "HTTP/", 5) != 0 || protocolData[6] != '.')
				return HTTPConnectionError::INCORRECT_PROTOCOL;

			/* Store HTTP Version */
			protocolData.push_back('\0');
			response->HTTPVersion = std::string(protocolData.data());

			/* Check if version is 'HTTP/1.1' */
			if (protocolData[5] != '1' || protocolData[7] != '1')
				Logger::Warning("HTTPConnection::ConsumeVersion", "HTTP Version isn't \"HTTP/1.1\": \"" + response->HTTPVersion + "\".");

			return HTTPConnectionError::NO_ERROR;
		}

		HTTPConnectionError HTTPConnection::ConsumeStatusCode(HTTPResponseInfo *response) {
			/* Read status code */
			std::vector<char> statusCode(4);
			size_t i;

			if (!ConnectionInfo.Read(statusCode.data(), 3))
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

				response->StatusCode = (statusCode[0] - 0x30) * 100
									 + (statusCode[1] - 0x30) * 10
									 + (statusCode[2] - 0x30);
			}

			return HTTPConnectionError::NO_ERROR;
		}

		/**
		 * Note: a reason phrase may be empty.
		 */
		HTTPConnectionError HTTPConnection::ConsumeReasonPhrase(HTTPResponseInfo *response) {
			std::vector<char> reasonPhrase;
			std::optional<char> character;
			while ((character = ConnectionInfo.ReadChar()).has_value()) {
				if (character == '\r') {
					reasonPhrase.push_back('\0');
					response->ReasonPhrase = reasonPhrase.data();
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
				response->ReasonPhrase = reasonPhrase.data();
				return HTTPConnectionError::INCORRECT_REASON_PHRASE;
			}

			reasonPhrase.push_back('\0');
			response->ReasonPhrase = reasonPhrase.data();
			return HTTPConnectionError::FAILED_READ_REASON_PHRASE;
		}

		HTTPConnectionError HTTPConnection::Request(HTTPResponseInfo *response, std::string method, std::string path) {
			std::optional<char> singleCharacter;
			std::stringstream request;
			HTTPConnectionError subroutineError;

			// TODO A stringstream isn't really needed at this point, so should
			// we use a vector for performance reasons?
			request << method << ' ' << path << " HTTP/1.1\r\n";
			request << "Host: " << ConnectionInfo.HostName << "\r\n";
			request << "\r\n";

			std::string str = request.str();
			if (!ConnectionInfo.Write(str.c_str(), str.length()))
				return HTTPConnectionError::FAILED_WRITE_REQUEST;

			/* Consume HTTP-Version */
			subroutineError = ConsumeHTTPVersion(response);
			if (subroutineError != HTTPConnectionError::NO_ERROR)
				return subroutineError;

			/* Consume space between 'HTTP-version' and 'status-code' */
			singleCharacter = ConnectionInfo.ReadChar();
			if (!singleCharacter.has_value())
				return HTTPConnectionError::FAILED_READ_GENERIC;

			if (singleCharacter.value() != ' ')
				return HTTPConnectionError::INCORRECT_START_LINE;

			/* Consume 'status-code' */
			subroutineError = ConsumeStatusCode(response);
			if (subroutineError != HTTPConnectionError::NO_ERROR)
				return subroutineError;

			/* Consume space between 'status-code' and 'reason-phrase' */
			singleCharacter = ConnectionInfo.ReadChar();
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
			singleCharacter = ConnectionInfo.ReadChar();
			if (!singleCharacter.has_value())
				return HTTPConnectionError::FAILED_READ_GENERIC;

			if (singleCharacter.value() != '\n')
				return HTTPConnectionError::INCORRECT_START_LINE;


			return HTTPConnectionError::NO_ERROR;
		}

		HTTPConnectionError HTTPConnection::RequestNavigation(HTTPResponseInfo *response, std::string path) {
			return Request(response, "GET", path);
		}
	}
}
