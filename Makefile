# Definition style for non-main binary:
#   bin/<folders>/<filename>.so: src/<location>/<filename>.cpp \
#   O	src/<location>/<filename>.hpp \
#   *	src/<dependency_location>/<dependency_filename>.hpp
#	$(CXX) $(CFLAGS) -c -o $@ src/<location>/<filename>.cpp
# Example:
#   bin/shapes/circle.so: src/shapes/circle.cpp \
#	src/shapes/circle.hpp \
#	src/shapes/shape.hpp \
#	src/shapes/position.hpp \
#	$(CXX) $(CFLAGS) -c -o $@ src/shapes/circle.cpp


CFLAGS = -Isrc -g -Wall -std=c++17 -O0
CXX = c++
LDFLAGS = `pkg-config --static --libs libtls`

BINARIES = bin/ccompat.so \
	   bin/logger.so \
	   bin/data/text/ustring.so \
	   bin/data/text/encoding/encoder_engine.so \
	   bin/data/text/encoding/single_byte_encoding.so \
	   bin/data/text/encoding/utf8.so \
	   bin/net/connection_info.so \
	   bin/net/connection_info_libtls.so \
	   bin/net/http/http_connection.so \
	   bin/net/http/http_response_info.so \
	   bin/parser/html/context.so \
	   bin/parser/html/error.so \
	   bin/parser/html/token.so \
	   bin/parser/html/tokenizer.so \
	   bin/parser/html/tree_constructor.so

engine: bin/test.txt \
	src/main.cpp \
	src/parser/html/error.hpp \
	src/parser/html/state.hpp \
	$(BINARIES)
	$(CXX) $(CFLAGS) -o $@ src/main.cpp $(BINARIES) $(LDFLAGS)

bin/test.txt:
	@mkdir -p bin/data/text/encoding
	@mkdir -p bin/net/http
	@mkdir -p bin/parser/html
	@touch bin/test.txt

bin/ccompat.so: src/ccompat.cpp src/ccompat.hpp
	$(CXX) $(CFLAGS) -c -o $@ src/ccompat.cpp

bin/logger.so: src/logger.cpp src/logger.hpp
	$(CXX) $(CFLAGS) -c -o $@ src/logger.cpp

bin/data/text/ustring.so: src/data/text/ustring.cpp \
	src/data/text/ustring.hpp \
	src/data/text/unicode.hpp
	$(CXX) $(CFLAGS) -c -o $@ src/data/text/ustring.cpp
	
bin/data/text/encoding/single_byte_encoding.so: src/data/text/encoding/single_byte_encoding.cpp \
	src/data/text/encoding/single_byte_encoding.hpp \
	src/data/text/encoding/encoding.hpp \
	src/data/text/unicode.hpp
	$(CXX) $(CFLAGS) -c -o $@ src/data/text/encoding/single_byte_encoding.cpp

bin/data/text/encoding/encoder_engine.so: src/data/text/encoding/encoder_engine.cpp \
	src/data/text/encoding/encoder_engine.hpp \
	src/data/text/encoding/single_byte_encoding.hpp \
	src/data/text/encoding/encoding.hpp \
	src/data/text/unicode.hpp
	$(CXX) $(CFLAGS) -c -o $@ src/data/text/encoding/encoder_engine.cpp

bin/data/text/encoding/utf8.so: src/data/text/encoding/utf8.cpp \
	src/data/text/encoding/utf8.hpp \
	src/data/text/encoding/encoding.hpp \
	src/data/text/unicode.hpp
	$(CXX) $(CFLAGS) -c -o $@ src/data/text/encoding/utf8.cpp

bin/net/connection_info.so: src/net/connection_info.cpp \
	src/net/connection_info.hpp \
	src/ccompat.hpp \
	src/logger.hpp
	$(CXX) $(CFLAGS) -c -o $@ src/net/connection_info.cpp

bin/net/connection_info_libtls.so: src/net/connection_info_libtls.cpp \
	src/net/connection_info.hpp \
	src/logger.hpp
	$(CXX) $(CFLAGS) -c -o $@ src/net/connection_info_libtls.cpp

bin/net/http/http_connection.so: src/net/http/http_connection.cpp \
	src/net/http/http_connection.hpp \
	src/logger.hpp \
	src/net/http/http_response_info.hpp \
	src/net/connection_info.hpp
	$(CXX) $(CFLAGS) -c -o $@ src/net/http/http_connection.cpp

bin/net/http/http_response_info.so: src/net/http/http_response_info.cpp \
	src/net/http/http_response_info.hpp \
	src/net/http/http_header_field.hpp
	$(CXX) $(CFLAGS) -c -o $@ src/net/http/http_response_info.cpp

bin/parser/html/context.so: src/parser/html/context.cpp \
	src/parser/html/context.hpp
	$(CXX) $(CFLAGS) -c -o $@ src/parser/html/context.cpp

bin/parser/html/error.so: src/parser/html/error.cpp \
	src/parser/html/error.hpp
	$(CXX) $(CFLAGS) -c -o $@ src/parser/html/error.cpp

bin/parser/html/token.so: src/parser/html/token.cpp \
	src/parser/html/token.hpp
	$(CXX) $(CFLAGS) -c -o $@ src/parser/html/token.cpp

bin/parser/html/tokenizer.so: src/parser/html/tokenizer.cpp \
	src/parser/html/tokenizer.cpp \
	src/parser/html/context.hpp \
	src/parser/html/error.hpp \
	src/parser/html/token.hpp \
	src/parser/html/tree_constructor.hpp \
	src/data/text/named_characters.hpp \
	src/data/text/unicode.hpp \
	src/data/text/ustring.hpp
	$(CXX) $(CFLAGS) -c -o $@ src/parser/html/tokenizer.cpp

bin/parser/html/tree_constructor.so: src/parser/html/tree_constructor.cpp \
	src/parser/html/tree_constructor.hpp
	$(CXX) $(CFLAGS) -c -o $@ src/parser/html/tree_constructor.cpp
