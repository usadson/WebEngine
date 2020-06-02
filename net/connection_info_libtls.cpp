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

#include "connection_info.hpp"

#include <chrono>
#include <iostream>
#include <sstream>
#include <vector>

#include <tls.h>

#include "logger.hpp"

namespace Net {

	void
	ConnectionInfo::TLSDestroy() {
		Logger::Debug(__PRETTY_FUNCTION__, "Called");
		tls_close((struct tls *) tlsContext);
		tls_free((struct tls *) tlsContext);
		tlsContext = nullptr;
	}

	bool
	ConnectionInfo::TLSRead(char *buf, size_t len) {
		do {
			ssize_t ret = tls_read((struct tls *) tlsContext, buf, len);
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

		if (tls_read((struct tls *) tlsContext, &character, 1) == -1)
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

		if (tls_config_set_alpn(config, tlsALPNProtocols.c_str()) == -1) {
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

		if (tls_connect_socket(context, socket, hostName.c_str()) == -1) {
			const char *error = tls_error(context);
			Logger::Warning("ConnectionInfo::TLSSetup[libtls]", std::string("Failed to setup secure connection with ") + hostName + ": " + error);
			tls_free(context);
			return false;
		}

		if (tls_handshake(context) == -1) {
			const char *error = tls_error(context);
			Logger::Warning("ConnectionInfo::TLSSetup[libtls]", std::string("Failed to setup secure connection (handshake) with ") + hostName + ": " + error);
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

		tlsContext = context;
		isAuthenticated = true;
		return true;
	}

	bool
	ConnectionInfo::TLSWrite(const char *buf, size_t len) {
		do {
			ssize_t ret = tls_write((struct tls *) tlsContext, buf, len);

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
