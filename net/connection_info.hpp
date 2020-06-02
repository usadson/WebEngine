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
