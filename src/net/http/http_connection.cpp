#include "http_connection.hpp" 

#include <sstream>

#include "logger.hpp"

namespace Net {
	namespace HTTP {
		HTTPConnection::HTTPConnection(Net::ConnectionInfo connectionInfo)
				: ConnectionInfo(connectionInfo) {
			if (!connectionInfo.Connect()) {
				std::stringstream information;
				information << "Failed to connect! Host: \"" << connectionInfo.HostName << "\":" << connectionInfo.Port;
				Logger::Error("HTTPConnection", information.str());
				return;
			}
		}

		HTTPConnection::~HTTPConnection() {
			
		}

		void HTTPConnection::Request(std::string path) {
			
		}
	}
}
