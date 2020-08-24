#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

#include <map>
#include <utility>

#include "http_response_info.hpp"
#include "net/connection_info.hpp"

#ifndef PRIVATE_VISIBILITY
#define PRIVATE_VISIBILITY private
#endif

namespace Net::HTTP {
	enum class HTTPConnectionError {
		FAILED_READ_GENERIC,
		FAILED_READ_HEADER_FIELD_GENERIC,
		FAILED_READ_HEADER_FIELD_NAME,
		FAILED_READ_HEADER_FIELD_VALUE,
		FAILED_READ_HTTP_VERSION,
		FAILED_READ_MESSAGE_BODY,
		FAILED_READ_REASON_PHRASE,
		FAILED_READ_STATUS_CODE,
		FAILED_WRITE_REQUEST,
		INVALID_CONTENT_LENGTH,
		INCORRECT_HEADER_FIELD_GENERIC,
		INCORRECT_HEADER_FIELD_NAME,
		INCORRECT_HEADER_FIELD_VALUE,
		INCORRECT_PROTOCOL,
		INCORRECT_REASON_PHRASE,
		INCORRECT_STATUS_CODE,
		NEWLINE_EXPECTED,
		NO_ERROR,
		NOT_CONNECTED,
		WHITESPACE_EXPECTED,
	};

	inline std::ostream &
	operator<<(std::ostream &stream, const HTTPConnectionError &type) noexcept {
		static const std::map<HTTPConnectionError, std::string> httpConnectionErrorNames = {
			{ HTTPConnectionError::FAILED_READ_GENERIC, "FAILED_READ_GENERIC" },
			{ HTTPConnectionError::FAILED_READ_HEADER_FIELD_GENERIC, "FAILED_READ_HEADER_FIELD_GENERIC" },
			{ HTTPConnectionError::FAILED_READ_HEADER_FIELD_NAME, "FAILED_READ_HEADER_FIELD_NAME" },
			{ HTTPConnectionError::FAILED_READ_HTTP_VERSION, "FAILED_READ_HTTP_VERSION" },
			{ HTTPConnectionError::FAILED_READ_HEADER_FIELD_VALUE, "FAILED_READ_HEADER_FIELD_VALUE" },
			{ HTTPConnectionError::FAILED_READ_REASON_PHRASE, "FAILED_READ_REASON_PHRASE" },
			{ HTTPConnectionError::FAILED_READ_STATUS_CODE, "FAILED_READ_STATUS_CODE" },
			{ HTTPConnectionError::FAILED_READ_MESSAGE_BODY, "FAILED_READ_MESSAGE_BODY" },
			{ HTTPConnectionError::FAILED_WRITE_REQUEST, "FAILED_WRITE_REQUEST" },
			{ HTTPConnectionError::INVALID_CONTENT_LENGTH, "INVALID_CONTENT_LENGTH" },
			{ HTTPConnectionError::INCORRECT_HEADER_FIELD_GENERIC, "INCORRECT_HEADER_FIELD_GENERIC" },
			{ HTTPConnectionError::INCORRECT_HEADER_FIELD_NAME, "INCORRECT_HEADER_FIELD_NAME" },
			{ HTTPConnectionError::INCORRECT_HEADER_FIELD_VALUE, "INCORRECT_HEADER_FIELD_VALUE" },
			{ HTTPConnectionError::INCORRECT_PROTOCOL, "INCORRECT_PROTOCOL" },
			{ HTTPConnectionError::INCORRECT_REASON_PHRASE, "INCORRECT_REASON_PHRASE" },
			{ HTTPConnectionError::INCORRECT_STATUS_CODE, "INCORRECT_STATUS_CODE" },
			{ HTTPConnectionError::NEWLINE_EXPECTED, "NEWLINE_EXPECTED" },
			{ HTTPConnectionError::NO_ERROR, "NO_ERROR" },
			{ HTTPConnectionError::NOT_CONNECTED, "NOT_CONNECTED" },
			{ HTTPConnectionError::WHITESPACE_EXPECTED, "WHITESPACE_EXPECTED" },
		};

		auto it = httpConnectionErrorNames.find(type);
		return stream << (it == std::end(httpConnectionErrorNames) ? "UNRECOGNIZED_TYPE" : it->second);
	}

	class HTTPConnection {
	  public: // Public Properties
		Net::ConnectionInfo &connectionInfo;

	  PRIVATE_VISIBILITY:
		HTTPResponseInfo *response { nullptr };

	  public: // Con/destructors
		// Constructors setup the connection using 'connectionInfo'.
		explicit HTTPConnection(Net::ConnectionInfo &);

	  PRIVATE_VISIBILITY:
		[[nodiscard]] HTTPConnectionError
		ConsumeHeaderField(char);

		[[nodiscard]] HTTPConnectionError
		ConsumeHeaderFieldName(std::vector<char> *);

		[[nodiscard]] HTTPConnectionError
		ConsumeHeaderFieldValue(std::vector<char> *);

		[[nodiscard]] HTTPConnectionError
		ConsumeHeaders();

		[[nodiscard]] HTTPConnectionError
		ConsumeHTTPVersion();

		[[nodiscard]] HTTPConnectionError
		ConsumeMessageBody();

		[[nodiscard]] HTTPConnectionError
		ConsumeNewLine();

		[[nodiscard]] HTTPConnectionError
		ConsumeReasonPhrase();

		[[nodiscard]] HTTPConnectionError
		ConsumeSingleSpace();

		[[nodiscard]] HTTPConnectionError
		ConsumeStatusCode();

		void
		TrimOWS(std::vector<char> &) const noexcept;

	  public: // Methods
		[[nodiscard]] HTTPConnectionError
		Request(HTTPResponseInfo *, const std::string &method, const std::string &path);

		[[nodiscard]] HTTPConnectionError
		RequestNavigation(HTTPResponseInfo *, const std::string &path);
	};
} // namespace Net::HTTP
