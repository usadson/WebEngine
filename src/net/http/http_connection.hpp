#pragma once

#include "net/connection_info.hpp"

namespace Net {
	namespace HTTP {
		class HTTPConnection {
		public: // Properties
			Net::ConnectionInfo ConnectionInfo;
		public: // Methods
			// Constructors setup the connection using 'connectionInfo'.
			HTTPConnection(Net::ConnectionInfo connectionInfo);
			~HTTPConnection();

			void Request(std::string path);
		};
	}
}
