# Copyright (C) 2020 Tristan <tristan@thewoosh.me>
#
# All Rights Reserved.
#
# Permission to use, copy, modify, and distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
#
# Definition style for non-main binary:
#   bin/<folders>/<filename>.so: <location>/<filename>.cpp \
#   O	<location>/<filename>.hpp \
#   *	<dependency_location>/<dependency_filename>.hpp
#	$(CXX) $(CFLAGS) -c -o $@ <location>/<filename>.cpp
# Example:
#   bin/shapes/circle.so: shapes/circle.cpp \
#	shapes/circle.hpp \
#	shapes/shape.hpp \
#	shapes/position.hpp \
#	$(CXX) $(CFLAGS) -c -o $@ shapes/circle.cpp

ADDITIONAL_CFLAGS ?=

GENERAL = -std=c++17 -g -Og
INCLUDES = -I.
WARNINGS = -Wall -Wextra -Wpedantic
CFLAGS += $(GENERAL) $(INCLUDES) $(WARNINGS) $(ADDITIONAL_CFLAGS)

CXX = clang++
LDFLAGS = `pkg-config --static --libs libtls glfw3 glew freetype2`

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
	   bin/net/http2/http2_connection.so \
	   bin/parser/html/context.so \
	   bin/parser/html/error.so \
	   bin/parser/html/token.so \
	   bin/parser/html/tokenizer.so \
	   bin/parser/html/tree/insert_before_head.so \
	   bin/parser/html/tree/insert_before_html.so \
	   bin/parser/html/tree/insert_intial.so \
	   bin/parser/html/tree_constructor.so \
	   bin/rendering/opengl/gl_renderer.so \
	   bin/rendering/window/window_glfw.so

all: bin/test.txt $(BINARIES) engine

clean:
	rm -rf bin
	rm -rf engine

engine: main.cpp \
	parser/html/error.hpp \
	parser/html/state.hpp
	$(CXX) $(CFLAGS) -o $@ main.cpp $(BINARIES) $(LDFLAGS)

bin/test.txt:
	@mkdir -p bin/data/text/encoding
	@mkdir -p bin/net/http
	@mkdir -p bin/net/http2
	@mkdir -p bin/parser/html/tree
	@mkdir -p bin/rendering/window
	@mkdir -p bin/rendering/opengl
	@mkdir -p bin/rendering/window
	@touch bin/test.txt

bin/ccompat.so: ccompat.cpp ccompat.hpp
	$(CXX) $(CFLAGS) -c -o $@ ccompat.cpp

bin/logger.so: logger.cpp logger.hpp
	$(CXX) $(CFLAGS) -c -o $@ logger.cpp

bin/data/text/ustring.so: data/text/ustring.cpp \
	data/text/ustring.hpp \
	data/text/unicode.hpp
	$(CXX) $(CFLAGS) -c -o $@ data/text/ustring.cpp

bin/data/text/encoding/single_byte_encoding.so: data/text/encoding/single_byte_encoding.cpp \
	data/text/encoding/single_byte_encoding.hpp \
	data/text/encoding/encoding.hpp \
	data/text/unicode.hpp
	$(CXX) $(CFLAGS) -c -o $@ data/text/encoding/single_byte_encoding.cpp

bin/data/text/encoding/encoder_engine.so: data/text/encoding/encoder_engine.cpp \
	data/text/encoding/encoder_engine.hpp \
	data/text/encoding/single_byte_encoding.hpp \
	data/text/encoding/encoding.hpp \
	data/text/unicode.hpp
	$(CXX) $(CFLAGS) -c -o $@ data/text/encoding/encoder_engine.cpp

bin/data/text/encoding/utf8.so: data/text/encoding/utf8.cpp \
	data/text/encoding/utf8.hpp \
	data/text/encoding/encoding.hpp \
	data/text/unicode.hpp
	$(CXX) $(CFLAGS) -c -o $@ data/text/encoding/utf8.cpp

bin/net/connection_info.so: net/connection_info.cpp \
	net/connection_info.hpp \
	ccompat.hpp \
	logger.hpp
	$(CXX) $(CFLAGS) -c -o $@ net/connection_info.cpp

bin/net/connection_info_libtls.so: net/connection_info_libtls.cpp \
	net/connection_info.hpp \
	logger.hpp
	$(CXX) $(CFLAGS) -c -o $@ net/connection_info_libtls.cpp

bin/net/http/http_connection.so: net/http/http_connection.cpp \
	net/http/http_connection.hpp \
	logger.hpp \
	net/http/http_response_info.hpp \
	net/connection_info.hpp
	$(CXX) $(CFLAGS) -c -o $@ net/http/http_connection.cpp

bin/net/http/http_response_info.so: net/http/http_response_info.cpp \
	net/http/http_response_info.hpp \
	net/http/http_header_field.hpp
	$(CXX) $(CFLAGS) -c -o $@ net/http/http_response_info.cpp

bin/net/http2/http2_connection.so: net/http2/http2_connection.cpp \
	net/http2/http2_connection.hpp \
	logger.hpp \
	net/http/http_response_info.hpp \
	net/connection_info.hpp
	$(CXX) $(CFLAGS) -c -o $@ net/http2/http2_connection.cpp

bin/parser/html/context.so: parser/html/context.cpp \
	parser/html/context.hpp
	$(CXX) $(CFLAGS) -c -o $@ parser/html/context.cpp

bin/parser/html/error.so: parser/html/error.cpp \
	parser/html/error.hpp
	$(CXX) $(CFLAGS) -c -o $@ parser/html/error.cpp

bin/parser/html/token.so: parser/html/token.cpp \
	parser/html/token.hpp
	$(CXX) $(CFLAGS) -c -o $@ parser/html/token.cpp

bin/parser/html/tokenizer.so: parser/html/tokenizer.cpp \
	parser/html/tokenizer.cpp \
	parser/html/context.hpp \
	parser/html/error.hpp \
	parser/html/token.hpp \
	parser/html/tree_constructor.hpp \
	data/text/named_characters.hpp \
	data/text/unicode.hpp \
	data/text/ustring.hpp
	$(CXX) $(CFLAGS) -c -o $@ parser/html/tokenizer.cpp

bin/parser/html/tree/insert_intial.so: parser/html/tree/insert_initial.cpp \
	parser/html/tree/insert_initial.hpp \
	parser/html/tree_constructor.hpp
	$(CXX) $(CFLAGS) -c -o $@ parser/html/tree/insert_initial.cpp

bin/parser/html/tree/insert_before_html.so: parser/html/tree/insert_before_html.cpp \
	parser/html/tree/insert_before_html.hpp \
	parser/html/tree_constructor.hpp
	$(CXX) $(CFLAGS) -c -o $@ parser/html/tree/insert_before_html.cpp

bin/parser/html/tree/insert_before_head.so: parser/html/tree/insert_before_head.cpp \
	parser/html/tree/insert_before_head.hpp \
	parser/html/tree_constructor.hpp
	$(CXX) $(CFLAGS) -c -o $@ parser/html/tree/insert_before_head.cpp

bin/parser/html/tree_constructor.so: parser/html/tree_constructor.cpp \
	parser/html/tree_constructor.hpp
	$(CXX) $(CFLAGS) -c -o $@ parser/html/tree_constructor.cpp

bin/rendering/opengl/gl_renderer.so: rendering/opengl/gl_renderer.cpp \
	rendering/opengl/gl_renderer.hpp \
	rendering/renderer.hpp \
	rendering/render_object.hpp \
	rendering/render_bounds.hpp
	$(CXX) $(CFLAGS) -c -o $@ rendering/opengl/gl_renderer.cpp

bin/rendering/window/window_glfw.so: rendering/window/window_glfw.cpp \
	rendering/window/window_glfw.hpp \
	rendering/window/window.hpp \
	data/text/ustring.hpp
	$(CXX) $(CFLAGS) -c -o $@ rendering/window/window_glfw.cpp

# the 'memory' target will invoke Valgrind, which will run the executable and
# can track memory usage. Memory leaks, double free()'s, use-after-free,
# uninitialised values, etc. can be found by using this tool.
memory:
	valgrind --num-callers=100 \
		 --leak-resolution=high \
		 --leak-check=full \
		 --track-origins=yes \
		 --show-leak-kinds=all \
		 --track-fds=yes \
		 ./engine

# the 'cppcheck' target will invoke the cppcheck program. This program 
# statically analyzes the code.
cppcheck:
	cppcheck -I. -q --verbose --std=c++17 --enable=all .

infer:
	infer run -- make

infer-clean:
	infer run -- make clean all
