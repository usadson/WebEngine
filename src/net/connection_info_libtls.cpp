#include "connection_info.hpp"

#include <chrono>
#include <iostream>
#include <sstream>
#include <vector>

#include <tls.h>

#include "logger.hpp"

namespace Net {

	static struct tls_config *TLSConfiguration;

	void ConnectionInfo::TLSDestroy() {
		
	}

	bool ConnectionInfo::TLSRead(char *buf, size_t len) {
		ssize_t ret;
		do {
			ret = tls_read((struct tls *) TLSContext, buf, len);
			if (ret == TLS_WANT_POLLIN || ret == TLS_WANT_POLLOUT)
				continue;

			if (ret == -1)
				return false;

			buf += ret;
			len -= ret;
		} while (len > 0);

		return true;
	}

	std::optional<char> ConnectionInfo::TLSReadChar() {
		return std::optional<char>();
	}

	bool ConnectionInfo::TLSSetup() {
		if (!TLSConfiguration) {
			TLSConfiguration = tls_config_new();
			if (!TLSConfiguration) {
				Logger::Severe("ConnectionInfo::TLSSetup[libtls]", "Failed to create configuration! (Out of memory!)");
				return false;
			}
		}

		struct tls *context = tls_client();
		if (!context) {
			Logger::Severe("ConnectionInfo::TLSSetup[libtls]", "Failed to create tls_client (Out of memory!)");
			return false;
		}

		if (tls_configure(context, TLSConfiguration) == -1) {
			const char *error = tls_config_error(TLSConfiguration);
			Logger::Severe("ConnectionInfo::TLSSetup[libtls]", std::string("Failed to configure TLS context: ") + error);
			return false;
		}

		if (tls_connect_socket(context, Socket, HostName.c_str()) == -1) {
			const char *error = tls_error(context);
			Logger::Warning("ConnectionInfo::TLSSetup[libtls]", std::string("Failed to setup secure connection with ") + HostName + ": " + error);
			return false;
		}

		if (tls_handshake(context) == -1) {
			const char *error = tls_error(context);
			Logger::Warning("ConnectionInfo::TLSSetup[libtls]", std::string("Failed to setup secure connection (handshake) with ") + HostName + ": " + error);
			return false;
		}

		TLSContext = context;
		return true;
	}

	bool ConnectionInfo::TLSWrite(const char *buf, size_t len) {
		ssize_t ret;
		do {
			ret = tls_write((struct tls *) TLSContext, buf, len);

			if (ret == TLS_WANT_POLLIN || ret == TLS_WANT_POLLOUT)
				continue;

			if (ret == -1)
				return false;

			buf += ret;
			len -= ret;
		} while (len > 0);

		return true;
	}
}
