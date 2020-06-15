#pragma once

/**
 * Copyright (C) 2020 Tristan. All Rights Reserved.
 * This file is licensed under the BSD 2-Clause license.
 * See the COPYING file for licensing information.
 */

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
			: hostName(inHostName), port(inPort), connected(false), secure(false), isAuthenticated(false),
			  tlsALPNProtocols(""), socket(0), tlsContext(nullptr) {
		}

		ConnectionInfo(const std::string &inHostName, uint16_t inPort, bool inSecure)
			: hostName(inHostName), port(inPort), connected(false), secure(inSecure), isAuthenticated(false),
			  tlsALPNProtocols(""), socket(0), tlsContext(nullptr) {
		}

		virtual
		~ConnectionInfo();

		virtual bool
		Connect();

		virtual bool
		Read(char *, std::size_t);

		virtual std::optional<char>
		ReadChar();

		bool
		ResolveHostName();

		virtual bool
		Write(const char *, std::size_t);

	  private: // Private Methods for different TLS implementations:
		void
		TLSDestroy();

		bool
		TLSRead(char *, std::size_t);

		std::optional<char>
		TLSReadChar();

		bool
		TLSSetup();

		bool
		TLSWrite(const char *, std::size_t);
	};
} // namespace Net
