#include "connection_info.hpp"

#include <chrono>
#include <iostream>
#include <sstream>
#include <vector>

#include <tls.h>

#include "global.hpp"
#include "logger.hpp"

namespace Net {

	namespace Global {
		struct tls_config *TLSConfiguration;

		bool SetupTLS() {
			TLSConfiguration = tls_config_new();

			if (!TLSConfiguration) {
				Logger::Severe("ConnectionInfo::TLSSetup[libtls]", "Failed to create configuration! (Out of memory!)");
				return false;
			}

			return true;
		}

		void DestroyTLS() {
			tls_config_free(TLSConfiguration);
		}
	}

	void ConnectionInfo::TLSDestroy() {
		tls_close((struct tls *) TLSContext);
		TLSContext = nullptr;
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
		char character;

		if (tls_read((struct tls *) TLSContext, &character, 1) == -1)
			return std::optional<char>();

		return std::optional<char>(character);
	}

	bool ConnectionInfo::TLSSetup() {
		struct tls *context = tls_client();
		if (!context) {
			Logger::Severe("ConnectionInfo::TLSSetup[libtls]", "Failed to create tls_client (Out of memory!)");
			return false;
		}

		if (tls_configure(context, Global::TLSConfiguration) == -1) {
			const char *error = tls_error(context);
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
