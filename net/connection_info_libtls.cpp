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

#include "connection_info.hpp"

#include <chrono>
#include <iostream>
#include <sstream>
#include <vector>

#include <tls.h>

#include "logger.hpp"
#include "options.hpp"

namespace Net {

	void
	ConnectionInfo::TLSDestroy() {
		Logger::Debug(__PRETTY_FUNCTION__, "Called");
		tls_close((struct tls *) TLSContext);
		tls_free((struct tls *) TLSContext);
		TLSContext = nullptr;
	}

	bool
	ConnectionInfo::TLSRead(char *buf, size_t len) {
		do {
			ssize_t ret = tls_read((struct tls *) TLSContext, buf, len);
			if (ret == TLS_WANT_POLLIN || ret == TLS_WANT_POLLOUT)
				continue;

			if (ret == -1)
				return false;

			buf += ret;
			len -= ret;
		} while (len > 0);

		return true;
	}

	std::optional<char>
	ConnectionInfo::TLSReadChar() {
		char character;

		if (tls_read((struct tls *) TLSContext, &character, 1) == -1)
			return std::optional<char>();

		return std::optional<char>(character);
	}

	bool
	ConnectionInfo::TLSSetup() {
		struct tls_config *config = tls_config_new();

		if (!config) {
			Logger::Severe("ConnectionInfo::TLSSetup[libtls]", "Failed to create configuration! (Out of memory!)");
			return false;
		}

		if (tls_config_set_alpn(config, TLSALPNProtocols.c_str()) == -1) {
			const char *error = tls_config_error(config);
			Logger::Severe("ConnectionInfo::TLSSetup[libtls]", std::string("Failed to set ALPN: ") + error);
			tls_config_free(config);
			return false;
		}

		struct tls *context = tls_client();
		if (!context) {
			Logger::Severe("ConnectionInfo::TLSSetup[libtls]", "Failed to create tls_client (Out of memory!)");
			tls_config_free(config);
			return false;
		}

		if (tls_configure(context, config) == -1) {
			const char *error = tls_error(context);
			Logger::Severe("ConnectionInfo::TLSSetup[libtls]", std::string("Failed to configure TLS context: ") + error);
			tls_free(context);
			tls_config_free(config);
			return false;
		}

		tls_config_free(config);
		config = nullptr;

		if (tls_connect_socket(context, Socket, HostName.c_str()) == -1) {
			const char *error = tls_error(context);
			Logger::Warning("ConnectionInfo::TLSSetup[libtls]", std::string("Failed to setup secure connection with ") + HostName + ": " + error);
			tls_free(context);
			return false;
		}

		if (tls_handshake(context) == -1) {
			const char *error = tls_error(context);
			Logger::Warning("ConnectionInfo::TLSSetup[libtls]", std::string("Failed to setup secure connection (handshake) with ") + HostName + ": " + error);
			tls_free(context);
			return false;
		}

		std::cout << "TLS Information:"
				  << "\nTLSVersion=" << tls_conn_version(context)
				  << "\nCipher=" << tls_conn_cipher(context)
				  << "\nCipherStrength=" << tls_conn_cipher_strength(context)
				  << "\nServerName=" << tls_conn_servername(context)
				  << "\nCertificateIssuer=" << tls_peer_cert_issuer(context)
				  << "\nCertificateSubject=" << tls_peer_cert_subject(context)
				  << "\nCertificateHash=" << tls_peer_cert_hash(context)
				  << std::endl;

		TLSContext = context;
		IsAuthenticated = true;
		return true;
	}

	bool
	ConnectionInfo::TLSWrite(const char *buf, size_t len) {
		do {
			ssize_t ret = tls_write((struct tls *) TLSContext, buf, len);

			if (ret == TLS_WANT_POLLIN || ret == TLS_WANT_POLLOUT)
				continue;

			if (ret == -1)
				return false;

			buf += ret;
			len -= ret;
		} while (len > 0);

		return true;
	}

}
