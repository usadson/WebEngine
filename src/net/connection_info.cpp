#include "connection_info.hpp"

#include <chrono>
#include <iostream>
#include <sstream>
#include <vector>

#include "ccompat.hpp"
#include "logger.hpp"

#include <arpa/inet.h>
#include <cstring>
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

namespace Net {
	ConnectionInfo::~ConnectionInfo() {
		if (Socket != 0) {
			if (close(Socket) == -1) {
				int errorCode = errno;
				std::stringstream information;
				information << "Error on close() socket: " << CCompat::GetErrnoName(errorCode) << " (" << errorCode << ")";
				Logger::Warning("Net::ConnectionInfo::~", information.str());
			}
			Socket = 0;
		}
	}

	
	std::string FormatTime(std::chrono::duration<int32_t, std::nano> nanoDuration) {
		std::stringstream durationStream;
		if (nanoDuration.count() > 1000) {
			auto microDuration = std::chrono::duration_cast<std::chrono::microseconds>(nanoDuration);
			if (microDuration.count() > 1000) {
				auto milliDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(microDuration);
				durationStream << milliDuration.count() << " ms";
			} else
				durationStream << microDuration.count() << " μs";
		} else
			durationStream << nanoDuration.count() << " ns";
		return durationStream.str();
	}

	bool ConnectionInfo::ResolveHostName() {
		struct addrinfo *address;
		struct addrinfo *result;

		auto startResolve = std::chrono::high_resolution_clock::now();
		int errorCode = getaddrinfo(HostName.c_str(), NULL, NULL, &result);
		if (errorCode != 0) {
			std::stringstream information;
			information << "Error on getaddrinfo(): " << CCompat::GetErrnoName(errorCode) << " (" << errorCode << ")";
			Logger::Warning("Net::ConnectionInfo::ResolveHostName", information.str());
			freeaddrinfo(result);
			return false;
		}

		for (address = result; address != NULL; address = address->ai_next) {
			char hostname[1025];
			errorCode = getnameinfo(address->ai_addr, address->ai_addrlen, hostname, 1025, NULL, 0, 0); 
			std::cout << "trying " << hostname << std::endl;
			Socket = socket(AF_INET, SOCK_STREAM, 0);
			if (Socket == -1) {
				errorCode = errno;
				std::stringstream information;
				information << "Error on socket(): " << CCompat::GetErrnoName(errorCode) << " (" << errorCode << ")";
				Logger::Warning("Net::ConnectionInfo::ResolveHostName", information.str());
				freeaddrinfo(result);
				return false;
			}

			auto startConnect = std::chrono::high_resolution_clock::now();
			
			struct sockaddr_in connectAddress;
			connectAddress.sin_addr.s_addr = ((struct sockaddr_in *) address->ai_addr)->sin_addr.s_addr;
			connectAddress.sin_family = AF_INET;
			connectAddress.sin_port = htons(Port);

			if (connect(Socket, (struct sockaddr*) &connectAddress, sizeof(struct sockaddr_in)) != -1) {
				TimingConnect = FormatTime(std::chrono::high_resolution_clock::now() - startConnect);
				// TimingDNS shouldn't count the time of the successful connect().
				TimingDNS = FormatTime(startConnect - startResolve);
				freeaddrinfo(result);
				return true;
			}

			close(Socket);
		}
		freeaddrinfo(result);

		Socket = 0;
		TimingConnect = "0 ms (failed)";
		TimingDNS = FormatTime(std::chrono::high_resolution_clock::now() - startResolve);
		return false;
	}

	bool ConnectionInfo::Connect() {
		if (Socket != 0) {
			Logger::Warning("Net::ConnectionInfo::Connect", "Connect already called!");
			return false;
		}

		if (!ResolveHostName()) {
			Logger::Warning("Net::ConnectionInfo::Connect", "Failed to connect!");
			return false;
		}

		// Other settings? Such as tls?
		return true;
	}
}
