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

#include <openssl/err.h>
#include <openssl/ssl.h>

#include "logger.hpp"

namespace Net {

	void
	ConnectionInfo::TLSDestroy() {
		Logger::Debug(static_cast<const char *>(__PRETTY_FUNCTION__), "Called");
		SSL_free(static_cast<SSL *>(ssl));
		SSL_CTX_free(static_cast<SSL_CTX *>(sslContext));
		ssl = nullptr;
		sslContext = nullptr;
	}

	bool
	ConnectionInfo::TLSRead(char *buf, std::size_t len) {
		do {
			auto ret = SSL_read(static_cast<SSL *>(ssl), buf, len);

			if (ret <= 0)
				return false;

			buf += ret;
			len -= ret;
		} while (len > 0);

		return true;
	}

	std::optional<char>
	ConnectionInfo::TLSReadChar() {
		char character;

		if (SSL_read(static_cast<SSL *>(ssl), &character, 1) == -1)
			return std::optional<char>();

		return std::optional<char>(character);
	}

	bool
	ConnectionInfo::TLSSetup() {
		// 		OpenSSL_add_all_algorithms();
		// 		SSL_load_error_strings();

		SSL_CTX *ctx = SSL_CTX_new(SSLv23_method());

		if (ctx == nullptr) {
			ERR_print_errors_fp(stderr);
			return false;
		}

		SSL *ssl = SSL_new(ctx);
		SSL_set_fd(ssl, socket);

		if (SSL_connect(ssl) == -1) {
			ERR_print_errors_fp(stderr);
			return false;
		}

		this->sslContext = ctx;
		this->ssl = ssl;

		isAuthenticated = true;
		return true;
	}

	bool
	ConnectionInfo::TLSWrite(const char *buf, std::size_t len) {
		do {
			auto ret = SSL_write(static_cast<SSL *>(ssl), buf, len);

			if (ret <= 0)
				return false;

			buf += ret;
			len -= ret;
		} while (len > 0);

		return true;
	}

} // namespace Net
