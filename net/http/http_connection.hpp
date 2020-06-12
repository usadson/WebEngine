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
		INCORRECT_HEADER_FIELD_GENERIC,
		INCORRECT_HEADER_FIELD_NAME,
		INCORRECT_HEADER_FIELD_VALUE,
		INCORRECT_PROTOCOL,
		INCORRECT_REASON_PHRASE,
		INCORRECT_START_LINE,
		NO_ERROR,
		NOT_CONNECTED,
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
			{ HTTPConnectionError::INCORRECT_HEADER_FIELD_GENERIC, "INCORRECT_HEADER_FIELD_GENERIC" },
			{ HTTPConnectionError::INCORRECT_HEADER_FIELD_NAME, "INCORRECT_HEADER_FIELD_NAME" },
			{ HTTPConnectionError::INCORRECT_HEADER_FIELD_VALUE, "INCORRECT_HEADER_FIELD_VALUE" },
			{ HTTPConnectionError::INCORRECT_REASON_PHRASE, "INCORRECT_REASON_PHRASE" },
			{ HTTPConnectionError::INCORRECT_START_LINE, "INCORRECT_START_LINE" },
			{ HTTPConnectionError::NO_ERROR, "NO_ERROR" },
			{ HTTPConnectionError::NOT_CONNECTED, "NOT_CONNECTED" },
		};

		auto it = httpConnectionErrorNames.find(type);
		return stream << (it == std::end(httpConnectionErrorNames) ? "UNRECOGNIZED_TYPE" : it->second);
	}

	class HTTPConnection {
	  public: // Public Properties
		Net::ConnectionInfo &connectionInfo;

	  private: // Private Properties
		HTTPResponseInfo *response;

	  public: // Con/destructors
		// Constructors setup the connection using 'connectionInfo'.
		explicit HTTPConnection(Net::ConnectionInfo &);

		~HTTPConnection();

#ifdef HTTP_CONNECTION_ENABLE_TESTING
	  public: // Public Methods (for testing)
#else
	  protected: // Protected Methods
#endif
		HTTPConnectionError
		ConsumeHeaderField(char);

		HTTPConnectionError
		ConsumeHTTPVersion();

		HTTPConnectionError
		ConsumeReasonPhrase();

		HTTPConnectionError
		ConsumeStatusCode();

		HTTPConnectionError
		ConsumeHeaderFieldName(std::vector<char> *);

		HTTPConnectionError
		ConsumeHeaderFieldValue(std::vector<char> *);

		HTTPConnectionError
		ConsumeWhiteSpace();

		HTTPConnectionError
		ConsumeHeaders();

		HTTPConnectionError
		ConsumeMessageBody();

		HTTPConnectionError
		ConsumeNewLine();

	  public: // Methods
		HTTPConnectionError
		Request(HTTPResponseInfo *, const std::string &method, const std::string &path);

		HTTPConnectionError
		RequestNavigation(HTTPResponseInfo *, const std::string &path);
	};
} // namespace Net::HTTP
