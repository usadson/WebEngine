# Copyright (C) 2020 Tristan
#
# All Rights Reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
# Definition style for non-main binary:
#   bin/<folders>/<filename>.o: <location>/<filename>.cpp \
#   O	<location>/<filename>.hpp \
#   *	<dependency_location>/<dependency_filename>.hpp
#	$(CXX) $(CXXFLAGS) -c -o $@ <location>/<filename>.cpp
# Example:
#   bin/shapes/circle.o: shapes/circle.cpp \
#	shapes/circle.hpp \
#	shapes/shape.hpp \
#	shapes/position.hpp \
#	$(CXX) $(CXXFLAGS) -c -o $@ shapes/circle.cpp

TESTING_CXXFLAGS = $(CXXFLAGS) -I..
TESTING_LDFLAGS = `pkg-config --static --libs gtest 2> /dev/null || echo -L/usr/local/lib -lgtest -lpthread` bin/logger.o


TESTING_INCLUDES_NET_HTTP = bin/logger.o \
			    bin/ccompat.o \
			    bin/net/connection_info.o \
			    $(CONNECTION_INFO_TLS_OBJECT) \
			    bin/net/http/http_connection.o \
			    bin/net/http/http_response_info.o

TESTING_INCLUDES_HTMLPARSER = bin/data/text/encoding/utf8.o \
			      bin/data/text/ustring.o \
			      bin/parser/html/context.o \
			      bin/parser/html/error.o \
			      bin/parser/html/state.o \
			      bin/parser/html/token.o \


TESTING_LDFLAGS_NET_HTTP = `pkg-config --static --libs gtest $(CONNECTION_INFO_TLS_PKGNAME)` \
			   $(TESTING_INCLUDES_NET_HTTP)

TESTING_LDFLAGS_CSSPARSER = $(TESTING_LDFLAGS) \
	bin/data/text/ustring.o \
	bin/data/text/encoding/utf8.o \
	bin/parser/css/preprocessor.o \

TESTING_LDFLAGS_HTMLPARSER = $(TESTING_LDFLAGS) $(TESTING_INCLUDES_HTMLPARSER)

TESTING_TARGETS = \
	bin/testing/directory.txt \
	bin/testing/data/text/encoding/utf8 \
	bin/testing/data/text/unicode \
	bin/testing/data/text/ustring \
	bin/testing/net/http/http_connection \
	bin/testing/net/http/http_response_info \
	bin/testing/parser/css/preprocessor \
	bin/testing/parser/html/token \


# the 'test' target executes every test
test:
	bin/testing/data/text/encoding/utf8
	bin/testing/data/text/unicode
	bin/testing/data/text/ustring
	bin/testing/net/http/http_connection
	bin/testing/net/http/http_response_info
	bin/testing/parser/html/token

bin/testing/directory.txt:
	@mkdir bin/testing
	@mkdir bin/testing/data
	@mkdir bin/testing/data/text
	@mkdir bin/testing/data/text/encoding
	@mkdir bin/testing/net
	@mkdir bin/testing/net/http
	@mkdir bin/testing/parser
	@mkdir bin/testing/parser/css
	@mkdir bin/testing/parser/html
	@touch bin/testing/directory.txt

bin/testing/data/text/encoding/utf8: testing/data/text/encoding/utf8.cpp \
	data/text/encoding/utf8.hpp \
	bin/data/text/encoding/utf8.o
	$(CXX) $(TESTING_CXXFLAGS) -o $@ testing/data/text/encoding/utf8.cpp bin/data/text/encoding/utf8.o $(TESTING_LDFLAGS)

bin/testing/data/text/unicode: testing/data/text/unicode.cpp \
	data/text/unicode.hpp
	$(CXX) $(TESTING_CXXFLAGS) -o $@ testing/data/text/unicode.cpp $(TESTING_LDFLAGS)

bin/testing/data/text/ustring: testing/data/text/ustring.cpp \
	data/text/ustring.hpp \
	bin/data/text/encoding/utf8.o \
	bin/data/text/ustring.o
	$(CXX) $(TESTING_CXXFLAGS) -o $@ testing/data/text/ustring.cpp bin/data/text/ustring.o bin/data/text/encoding/utf8.o $(TESTING_LDFLAGS)

bin/testing/net/http/http_connection: testing/net/http/http_connection.cpp \
	net/connection_info.hpp \
	net/http/http_connection.hpp \
	logger.hpp \
	$(TESTING_INCLUDES_NET_HTTP)
	$(CXX) $(TESTING_CXXFLAGS) $(CONNECTION_INFO_TLS_IMPL) -o $@ testing/net/http/http_connection.cpp $(TESTING_LDFLAGS_NET_HTTP)

bin/testing/net/http/http_response_info: testing/net/http/http_response_info.cpp \
	net/connection_info.hpp \
	net/http/http_response_info.cpp \
	net/http/http_response_info.hpp \
	net/http/http_header_field.hpp \
	logger.hpp \
	bin/net/http/http_response_info.o
	$(CXX) $(TESTING_CXXFLAGS) -o $@ testing/net/http/http_response_info.cpp $(TESTING_LDFLAGS_NET_HTTP)

bin/testing/parser/css/preprocessor: testing/parser/css/preprocessor.cpp \
	bin/parser/css/preprocessor.o \
	data/text/ustring.hpp
	$(CXX) $(TESTING_CXXFLAGS) -o $@ testing/parser/css/preprocessor.cpp $(TESTING_LDFLAGS_CSSPARSER)

bin/testing/parser/html/token: testing/parser/html/token.cpp \
	parser/html/token.hpp \
	parser/html/token.cpp
	$(CXX) $(TESTING_CXXFLAGS) -o $@ testing/parser/html/token.cpp $(TESTING_LDFLAGS_HTMLPARSER)

