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

#pragma once

#include <optional>
#include <string>

#include <cstdint>

namespace Net {
	class ConnectionInfo {
	public: // Properties
		/* The HostName, as entered by the client. */
		std::string hostName;

		/* The port it's connected to (usually 443 or 80). */
		uint16_t port;

		/* Has a connection been established? (Note that r/w calls can still
		 * fail!) */
		bool connected;

		/* Should the connection be wrapped in TLS? */
		bool secure;

		/* Is 'Secure' and SetupTLS() was succesful and DestroyTLS() hasn't
		 * been called? */
		bool isAuthenticated;

		/* https://www.iana.org/assignments/tls-extensiontype-values/tls-extensiontype-values.xhtml#alpn-protocol-ids */
		std::string tlsALPNProtocols;

		/* The time it took to resolve the hostname. */
		std::string timingDNS;

		/* The time that was wasted trying to find a connectable server.
		 * (TimingConnect excluded) */
		std::string timingPing;

		/* The time it took to connect to the currently-connnected-to server. */
		std::string timingConnect;

		/* The time it took to setup the TLS context (handshaking etc.) */
		std::string timingTLS;
	private: // Private Properties
		int socket;
		void *tlsContext;
	public: // Methods
		ConnectionInfo(const std::string &inHostName, uint16_t inPort)
			: hostName(inHostName), port(inPort), connected(false),
			  secure(false), isAuthenticated(false), tlsALPNProtocols(""),
			  socket(0), tlsContext(nullptr) {
		}

		ConnectionInfo(const std::string &inHostName, uint16_t inPort, bool inSecure)
			: hostName(inHostName), port(inPort), connected(false),
			  secure(inSecure), isAuthenticated(false), tlsALPNProtocols(""),
			  socket(0), tlsContext(nullptr) {
		}

		~ConnectionInfo();

		bool
		Connect();

		bool
		Read(char *, size_t);

		std::optional<char>
		ReadChar();

		bool
		ResolveHostName();

		bool
		Write(const char *, size_t);
	private: // Private Methods for different TLS implementations:
		void
		TLSDestroy();

		bool
		TLSRead(char *, size_t);

		std::optional<char>
		TLSReadChar();

		bool
		TLSSetup();

		bool
		TLSWrite(const char *, size_t);
	};
}
