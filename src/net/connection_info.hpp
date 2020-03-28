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

		/* Is the connection wrapped in TLS? */
		bool Secure;

		/* The time it took to resolve the hostname. */
		std::string TimingDNS;

		/* The time that was wasted trying to find a connectable server.
		 * (TimingConnect excluded) */
		std::string TimingPing;

		/* The time it took to connect to the currently-connnected-to server. */
		std::string TimingConnect;
	private: // Private Properties
		int Socket;
	public: // Methods
		ConnectionInfo(std::string hostName, uint16_t port)
			: HostName(hostName), Port(port), Secure(false), Socket(0) {
		}

		ConnectionInfo(std::string hostName, uint16_t port, bool secure)
			: HostName(hostName), Port(port), Secure(secure), Socket(0) {
		}

		~ConnectionInfo();

		bool Connect();
		bool Read(char *data, size_t length);
		std::optional<char> ReadChar();
		bool ResolveHostName();
		bool Write(const char *data, size_t length);
	};
}
