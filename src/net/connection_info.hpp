#pragma once

#include <string>

#include <cstdint>

namespace Net {
	class ConnectionInfo {
	public: // Properties
		std::string HostName;
		uint16_t Port;
		bool Secure;

		std::string TimingDNS;
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

		bool ResolveHostName();
		bool Connect();
	};
}
