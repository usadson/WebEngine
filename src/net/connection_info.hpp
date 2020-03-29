#pragma once

#include <optional>
#include <string>

#include <cstdint>

namespace Net {
	class ConnectionInfo {
	public: // Properties
		/* The HostName, as entered by the client. */
		std::string HostName;

		/* The port it's connected to (usually 443 or 80). */
		uint16_t Port;

		/* Has a connection been established? (Note that r/w calls can still
		 * fail!) */
		bool Connected;

		/* Should the connection be wrapped in TLS? */
		bool Secure;

		/* Is 'Secure' and SetupTLS() was succesful and DestroyTLS() hasn't
		 * been called? */
		bool IsAuthenticated;

		/* The time it took to resolve the hostname. */
		std::string TimingDNS;

		/* The time that was wasted trying to find a connectable server.
		 * (TimingConnect excluded) */
		std::string TimingPing;

		/* The time it took to connect to the currently-connnected-to server. */
		std::string TimingConnect;

		/* The time it took to setup the TLS context (handshaking etc.) */
		std::string TimingTLS;
	private: // Private Properties
		int Socket;
		void *TLSContext;
	public: // Methods
		ConnectionInfo(std::string hostName, uint16_t port)
			: HostName(hostName), Port(port), Connected(false), Secure(false), IsAuthenticated(false), Socket(0) {
		}

		ConnectionInfo(std::string hostName, uint16_t port, bool secure)
			: HostName(hostName), Port(port), Connected(false), Secure(secure), IsAuthenticated(false), Socket(0) {
		}

		~ConnectionInfo();

		bool Connect();
		bool Read(char *data, size_t length);
		std::optional<char> ReadChar();
		bool ResolveHostName();
		bool Write(const char *data, size_t length);
	private: // Private Methods for different TLS implementations:
		void TLSDestroy();
		bool TLSRead(char *data, size_t length);
		std::optional<char> TLSReadChar();
		bool TLSSetup();
		bool TLSWrite(const char *data, size_t length);
	};
}
