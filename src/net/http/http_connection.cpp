#include "http_connection.hpp" 

#include <sstream>
#include <vector>

#include <cstring>

#include "logger.hpp"

namespace Net {
	namespace HTTP {
		HTTPConnection::HTTPConnection(Net::ConnectionInfo connectionInfo)
				: ConnectionInfo(connectionInfo) {
			if (!ConnectionInfo.Connect()) {
				std::stringstream information;
				information << "Failed to connect! Host: \"" << connectionInfo.HostName << "\":" << connectionInfo.Port;
				Logger::Error("HTTPConnection", information.str());
				return;
			}
		}

		HTTPConnection::~HTTPConnection() {
			
		}

		HTTPConnectionError HTTPConnection::Request(HTTPResponseInfo *response, std::string method, std::string path) {
			// TODO A stringstream isn't really needed at this point, so should
			// we use a vector for performance reasons?
			std::stringstream request;
			request << method << ' ' << path << " HTTP/1.1\r\n";
			request << "Host: " << ConnectionInfo.HostName << "\r\n";
			request << "\r\n";

			std::string str = request.str();
			if (!ConnectionInfo.Write(str.c_str(), str.length()))
				return HTTPConnectionError::FAILED_WRITE_REQUEST;

			/* Read protocol version (= HTTP-version) */
			std::vector<char> protocolData(9);
			if (!ConnectionInfo.Read(protocolData.data(), 8))
				return HTTPConnectionError::FAILED_READ_VERSION;

			/* Validate HTTP Version */
			if (!memcmp(protocolData.data(), "HTTP/", 5) || protocolData[6] != '.')
				return HTTPConnectionError::INCORRECT_PROTOCOL;

			/* Store HTTP Version */
			protocolData.push_back('\0');
			response->HTTPVersion = std::string(protocolData.data());

			/* Check if version is 'HTTP/1.1' */
			if (protocolData[5] != '1' || protocolData[7] != '1') {
				Logger::Warning("HTTPConnection::Request", "HTTP Version isn't \"HTTP/1.1\": \"" + response->HTTPVersion + "\".");
			}

			return HTTPConnectionError::NO_ERROR;
		}

		HTTPConnectionError HTTPConnection::RequestNavigation(HTTPResponseInfo *response, std::string path) {
			return Request(response, "GET", path);
		}
	}
}
