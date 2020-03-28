#include "connection_info.hpp"

#include <iostream>
#include <sstream>
#include <vector>

#include "ccompat.hpp"
#include "logger.hpp"

#include <arpa/inet.h>
#include <cstring>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

namespace Net {
	ConnectionInfo::~ConnectionInfo() {
		if (Socket != 0) {
			if (close(Socket) == -1) {
				int errorCode = errno;
				std::stringstream information;
				information << "Error on close() socket: " << CCompat::GetErrnoName(errorCode) << " (" << errorCode << ")";
				Logger::Warning("Net::ConnectionInfo", information.str());
			}
			Socket = 0;
		}
	}

	bool ConnectionInfo::Connect() {
		if (Socket != 0) {
			Logger::Warning("Net::ConnectionInfo", "ConnectionInfo::Connect already called!");
			return false;
		}

		Socket = -1;//ocket(AF_INET, SOCK_STREAM, 0);

		return false;
	}
}
