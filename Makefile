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

.DEFAULT_GOAL := all

ADDITIONAL_CXXFLAGS ?=

GENERAL = -std=c++17 -g
			# -Og
INCLUDES = -I.
WARNINGS = -Wall -Wextra -Wpedantic
CXXFLAGS += $(GENERAL) $(INCLUDES) $(WARNINGS) $(ADDITIONAL_CXXFLAGS)
CXX = clang++
#LDFLAGS = `pkg-config --static --libs libtls glfw3 glew freetype2`
LDFLAGS = `pkg-config --static --libs openssl glfw3 glew freetype2`

CONNECTION_INFO_TLS_IMPL = -DCONNECTION_INFO_TLS_IMPL_OPENSSL
CONNECTION_INFO_TLS_OBJECT = bin/net/connection_info_openssl.o

# All the object files. By convention, each .cpp should have a corresponding
# object file. For more information, see the explanation above.
BINARIES = bin/ccompat.o \
	   bin/logger.o \
	   bin/options.o \
	   bin/data/text/named_characters.so \
	   bin/data/text/ustring.o \
	   bin/data/text/encoding/encoder_engine.o \
	   bin/data/text/encoding/single_byte_encoding.o \
	   bin/data/text/encoding/utf8.o \
	   bin/misc/credits.o \
	   bin/net/connection_info.o \
	   $(CONNECTION_INFO_TLS_OBJECT) \
	   bin/net/http/http_connection.o \
	   bin/net/http/http_response_info.o \
	   bin/net/http2/http2_connection.o \
	   bin/parser/html/context.o \
	   bin/parser/html/error.o \
	   bin/parser/html/insertion_mode.o \
	   bin/parser/html/state.o \
	   bin/parser/html/token.o \
	   bin/parser/html/tokenizer.o \
	   bin/parser/html/tokenizer/after_attribute_name.o \
	   bin/parser/html/tokenizer/after_attribute_value_qouted.o \
	   bin/parser/html/tokenizer/after_doctype_name.o \
	   bin/parser/html/tokenizer/after_doctype_public_identifier.o \
	   bin/parser/html/tokenizer/after_doctype_public_keyword.o \
	   bin/parser/html/tokenizer/after_doctype_system_identifier.o \
	   bin/parser/html/tokenizer/after_doctype_system_keyword.o \
	   bin/parser/html/tokenizer/attribute_name.o \
	   bin/parser/html/tokenizer/attribute_value_dq.o \
	   bin/parser/html/tokenizer/attribute_value_nq.o \
	   bin/parser/html/tokenizer/attribute_value_sq.o \
	   bin/parser/html/tokenizer/before_attribute_name.o \
	   bin/parser/html/tokenizer/before_attribute_value.o \
	   bin/parser/html/tokenizer/before_doctype_name.o \
	   bin/parser/html/tokenizer/before_doctype_public_identifier.o \
	   bin/parser/html/tokenizer/before_doctype_system_identifier.o \
	   bin/parser/html/tokenizer/between_doctype_public_system_identifier.o \
	   bin/parser/html/tokenizer/bogus_doctype.o \
	   bin/parser/html/tokenizer/character_reference.o \
	   bin/parser/html/tokenizer/comment_end_bang.o \
	   bin/parser/html/tokenizer/comment_end_dash.o \
	   bin/parser/html/tokenizer/comment_end.o \
	   bin/parser/html/tokenizer/comment_lts_bang_dash_dash.o \
	   bin/parser/html/tokenizer/comment_lts_bang_dash.o \
	   bin/parser/html/tokenizer/comment_lts_bang.o \
	   bin/parser/html/tokenizer/comment_lts.o \
	   bin/parser/html/tokenizer/comment_start.o \
	   bin/parser/html/tokenizer/comment_start_dash.o \
	   bin/parser/html/tokenizer/comment.o \
	   bin/parser/html/tokenizer/data.o \
	   bin/parser/html/tokenizer/doctype_name.o \
	   bin/parser/html/tokenizer/doctype_public_identifier_dq.o \
	   bin/parser/html/tokenizer/doctype_public_identifier_sq.o \
	   bin/parser/html/tokenizer/doctype_system_identifier_dq.o \
	   bin/parser/html/tokenizer/doctype_system_identifier_sq.o \
	   bin/parser/html/tokenizer/doctype.o \
	   bin/parser/html/tokenizer/markup_declaration_open.o \
	   bin/parser/html/tokenizer/self_closing_start.o \
	   bin/parser/html/tokenizer/tag_end_open.o \
	   bin/parser/html/tokenizer/tag_name.o \
	   bin/parser/html/tokenizer/tag_open.o \
	   bin/parser/html/tree/insert_before_head.o \
	   bin/parser/html/tree/insert_before_html.o \
	   bin/parser/html/tree/insert_in_head.o \
	   bin/parser/html/tree/insert_intial.o \
	   bin/parser/html/tree_constructor.o \
	   bin/rendering/opengl/gl_renderer.o \
	   bin/rendering/window/window_glfw.o

include testing/Makefile

# The 'all' target will compile all object files and generate the binary
# executable. This is the default target for 'make'.
all: bin/test.txt $(BINARIES) $(TESTING_TARGETS) engine

# The 'objects' target will compile all object files, but not generate the
# binary executable.
objects: bin/test.txt $(BINARIES)

# The 'clean' target will remove all binaries generated by the Makefile/compiler.
# This will restore the state to a clean git clone.
clean:
	rm -rf bin
	rm -rf engine

# The 'engine' target will build the final binary executable.
engine: main.cpp \
	parser/html/error.hpp \
	parser/html/state.hpp $(BINARIES)
	$(CXX) $(CXXFLAGS) $(CONNECTION_INFO_TLS_IMPL) -o $@ main.cpp $(BINARIES) $(LDFLAGS)

# The 'bin/test.txt' will ensure all directories required by the object files
# are present. The bin/test.txt file will be touch'ed so that mkdir will only
# be called once.
#
# When getting a 'directory does not exists' error e.g. after pulling from
# origin, creating the directory yourself can be a hassle, so maybe just execute
# the 'clean' target.
bin/test.txt:
	@mkdir bin
	@mkdir bin/data
	@mkdir bin/data/text
	@mkdir bin/data/text/encoding
	@mkdir bin/misc
	@mkdir bin/net
	@mkdir bin/net/http
	@mkdir bin/net/http2
	@mkdir bin/parser
	@mkdir bin/parser/html
	@mkdir bin/parser/html/tokenizer
	@mkdir bin/parser/html/tree
	@mkdir bin/rendering
	@mkdir bin/rendering/window
	@mkdir bin/rendering/opengl
	@touch bin/test.txt

bin/ccompat.o: ccompat.cpp\
	ccompat.hpp \
	logger.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ ccompat.cpp

bin/logger.o: logger.cpp\
	logger.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ logger.cpp

bin/options.o: options.cpp \
	options.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ options.cpp

bin/data/text/named_characters.so: data/text/named_characters.cpp \
	data/text/named_characters.hpp \
	data/text/unicode.hpp \
	data/text/ustring.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ data/text/named_characters.cpp

bin/data/text/ustring.o: data/text/ustring.cpp \
	data/text/ustring.hpp \
	data/text/unicode.hpp \
	data/text/encoding/utf8.hpp \
	logger.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ data/text/ustring.cpp

bin/data/text/encoding/encoder_engine.o: data/text/encoding/encoder_engine.cpp \
	data/text/encoding/encoder_engine.hpp \
	data/text/encoding/single_byte_encoding.hpp \
	data/text/encoding/utf8.hpp \
	data/text/encoding/encoding.hpp \
	data/text/unicode.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ data/text/encoding/encoder_engine.cpp

bin/data/text/encoding/single_byte_encoding.o: data/text/encoding/single_byte_encoding.cpp \
	data/text/encoding/single_byte_encoding.hpp \
	data/text/encoding/encoding.hpp \
	data/text/unicode.hpp \
	logger.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ data/text/encoding/single_byte_encoding.cpp

bin/data/text/encoding/utf8.o: data/text/encoding/utf8.cpp \
	data/text/encoding/utf8.hpp \
	data/text/encoding/encoding.hpp \
	data/text/unicode.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ data/text/encoding/utf8.cpp

bin/misc/credits.o: misc/credits.cpp \
	misc/credits.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ misc/credits.cpp

bin/net/connection_info.o: net/connection_info.cpp \
	net/connection_info.hpp \
	ccompat.hpp \
	logger.hpp
	$(CXX) $(CXXFLAGS) $(CONNECTION_INFO_TLS_IMPL) -c -o $@ net/connection_info.cpp

#bin/net/connection_info_libtls.o: net/connection_info_libtls.cpp \
#	net/connection_info.hpp \
#	logger.hpp
#	$(CXX) $(CXXFLAGS) $(CONNECTION_INFO_TLS_IMPL) -c -o $@ net/connection_info_libtls.cpp

bin/net/connection_info_openssl.o: net/connection_info_openssl.cpp \
	net/connection_info.hpp \
	logger.hpp
	$(CXX) $(CXXFLAGS) $(CONNECTION_INFO_TLS_IMPL) -c -o $@ net/connection_info_openssl.cpp

bin/net/http/http_connection.o: net/http/http_connection.cpp \
	net/http/http_connection.hpp \
	net/http/http_response_info.hpp \
	net/connection_info.hpp \
	logger.hpp
	$(CXX) $(CXXFLAGS) $(CONNECTION_INFO_TLS_IMPL) -c -o $@ net/http/http_connection.cpp

bin/net/http/http_response_info.o: net/http/http_response_info.cpp \
	net/http/http_response_info.hpp \
	net/http/http_header_field.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ net/http/http_response_info.cpp

bin/net/http2/http2_connection.o: net/http2/http2_connection.cpp \
	net/http2/http2_connection.hpp \
	net/http2/constants.hpp \
	net/http2/frame.hpp \
	net/http/http_response_info.hpp \
	net/connection_info.hpp \
	logger.hpp
	$(CXX) $(CXXFLAGS) $(CONNECTION_INFO_TLS_IMPL) -c -o $@ net/http2/http2_connection.cpp

bin/parser/html/context.o: parser/html/context.cpp \
	parser/html/context.hpp \
	dom/document.hpp \
	parser/html/error.hpp \
	parser/html/state.hpp \
	parser/html/token.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/context.cpp

bin/parser/html/error.o: parser/html/error.cpp \
	parser/html/error.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/error.cpp

bin/parser/html/insertion_mode.o: parser/html/insertion_mode.cpp \
	parser/html/insertion_mode.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/insertion_mode.cpp

bin/parser/html/state.o: parser/html/state.cpp \
	parser/html/state.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/state.cpp

bin/parser/html/token.o: parser/html/token.cpp \
	parser/html/token.hpp \
	data/text/unicode.hpp \
	data/text/ustring.hpp \
	parser/html/context.hpp \
	logger.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/token.cpp

bin/parser/html/tokenizer.o: parser/html/tokenizer.cpp \
	parser/html/tokenizer.cpp \
	parser/html/tokenizer/tokenizer_parser.hpp \
	parser/html/context.hpp \
	parser/html/error.hpp \
	parser/html/token.hpp \
	parser/html/tree_constructor.hpp \
	data/text/named_characters.hpp \
	data/text/unicode.hpp \
	data/text/ustring.hpp \
	logger.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tokenizer.cpp

bin/parser/html/tree_constructor.o: parser/html/tree_constructor.cpp \
	parser/html/tree_constructor.hpp \
	dom/document.hpp \
	dom/element.hpp \
	dom/resettable_element.hpp \
	parser/html/tree/insert_before_head.hpp \
	parser/html/tree/insert_before_html.hpp \
	parser/html/tree/insert_initial.hpp \
	parser/html/tree/insertion_mode.hpp \
	parser/html/insertion_mode.hpp \
	parser/html/token.hpp \
	logger.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tree_constructor.cpp

bin/parser/html/tokenizer/after_attribute_name.o: parser/html/tokenizer/after_attribute_name.cpp \
	parser/html/tokenizer/after_attribute_name.hpp \
	parser/html/context.hpp \
	parser/html/token.hpp \
	parser/html/tokenizer.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tokenizer/after_attribute_name.cpp

bin/parser/html/tokenizer/after_attribute_value_qouted.o: parser/html/tokenizer/after_attribute_value_quoted.cpp \
	parser/html/tokenizer/after_attribute_value_quoted.hpp \
	parser/html/context.hpp \
	parser/html/token.hpp \
	parser/html/tokenizer.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tokenizer/after_attribute_value_quoted.cpp

bin/parser/html/tokenizer/after_doctype_name.o: parser/html/tokenizer/after_doctype_name.cpp \
	parser/html/tokenizer/after_doctype_name.hpp \
	parser/html/context.hpp \
	parser/html/token.hpp \
	parser/html/tokenizer.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tokenizer/after_doctype_name.cpp

bin/parser/html/tokenizer/after_doctype_public_identifier.o: parser/html/tokenizer/after_doctype_public_identifier.cpp \
	parser/html/tokenizer/after_doctype_public_identifier.hpp \
	parser/html/context.hpp \
	parser/html/token.hpp \
	parser/html/tokenizer.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tokenizer/after_doctype_public_identifier.cpp

bin/parser/html/tokenizer/after_doctype_public_keyword.o: parser/html/tokenizer/after_doctype_public_keyword.cpp \
	parser/html/tokenizer/after_doctype_public_keyword.hpp \
	parser/html/context.hpp \
	parser/html/token.hpp \
	parser/html/tokenizer.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tokenizer/after_doctype_public_keyword.cpp

bin/parser/html/tokenizer/after_doctype_system_identifier.o: parser/html/tokenizer/after_doctype_system_identifier.cpp \
	parser/html/tokenizer/after_doctype_system_identifier.hpp \
	parser/html/context.hpp \
	parser/html/token.hpp \
	parser/html/tokenizer.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tokenizer/after_doctype_system_identifier.cpp

bin/parser/html/tokenizer/after_doctype_system_keyword.o: parser/html/tokenizer/after_doctype_system_keyword.cpp \
	parser/html/tokenizer/after_doctype_system_keyword.hpp \
	parser/html/context.hpp \
	parser/html/token.hpp \
	parser/html/tokenizer.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tokenizer/after_doctype_system_keyword.cpp

bin/parser/html/tokenizer/attribute_name.o: parser/html/tokenizer/attribute_name.cpp \
	parser/html/tokenizer/attribute_name.hpp \
	parser/html/context.hpp \
	parser/html/token.hpp \
	parser/html/tokenizer.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tokenizer/attribute_name.cpp

bin/parser/html/tokenizer/attribute_value_dq.o: parser/html/tokenizer/attribute_value_dq.cpp \
	parser/html/tokenizer/attribute_value_dq.hpp \
	parser/html/context.hpp \
	parser/html/token.hpp \
	parser/html/tokenizer.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tokenizer/attribute_value_dq.cpp

bin/parser/html/tokenizer/attribute_value_nq.o: parser/html/tokenizer/attribute_value_nq.cpp \
	parser/html/tokenizer/attribute_value_nq.hpp \
	parser/html/context.hpp \
	parser/html/token.hpp \
	parser/html/tokenizer.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tokenizer/attribute_value_nq.cpp

bin/parser/html/tokenizer/attribute_value_sq.o: parser/html/tokenizer/attribute_value_sq.cpp \
	parser/html/tokenizer/attribute_value_sq.hpp \
	parser/html/context.hpp \
	parser/html/token.hpp \
	parser/html/tokenizer.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tokenizer/attribute_value_sq.cpp

bin/parser/html/tokenizer/before_attribute_name.o: parser/html/tokenizer/before_attribute_name.cpp \
	parser/html/tokenizer/before_attribute_name.hpp \
	parser/html/context.hpp \
	parser/html/token.hpp \
	parser/html/tokenizer.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tokenizer/before_attribute_name.cpp

bin/parser/html/tokenizer/before_attribute_value.o: parser/html/tokenizer/before_attribute_value.cpp \
	parser/html/tokenizer/before_attribute_value.hpp \
	parser/html/context.hpp \
	parser/html/token.hpp \
	parser/html/tokenizer.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tokenizer/before_attribute_value.cpp

bin/parser/html/tokenizer/before_doctype_name.o: parser/html/tokenizer/before_doctype_name.cpp \
	parser/html/tokenizer/before_doctype_name.hpp \
	parser/html/context.hpp \
	parser/html/token.hpp \
	parser/html/tokenizer.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tokenizer/before_doctype_name.cpp

bin/parser/html/tokenizer/before_doctype_public_identifier.o: parser/html/tokenizer/before_doctype_public_identifier.cpp \
	parser/html/tokenizer/before_doctype_public_identifier.hpp \
	parser/html/context.hpp \
	parser/html/token.hpp \
	parser/html/tokenizer.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tokenizer/before_doctype_public_identifier.cpp

bin/parser/html/tokenizer/before_doctype_system_identifier.o: parser/html/tokenizer/before_doctype_system_identifier.cpp \
	parser/html/tokenizer/before_doctype_system_identifier.hpp \
	parser/html/context.hpp \
	parser/html/token.hpp \
	parser/html/tokenizer.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tokenizer/before_doctype_system_identifier.cpp

bin/parser/html/tokenizer/between_doctype_public_system_identifier.o: parser/html/tokenizer/between_doctype_public_system_identifier.cpp \
	parser/html/tokenizer/between_doctype_public_system_identifier.hpp \
	parser/html/context.hpp \
	parser/html/token.hpp \
	parser/html/tokenizer.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tokenizer/between_doctype_public_system_identifier.cpp

bin/parser/html/tokenizer/bogus_doctype.o: parser/html/tokenizer/bogus_doctype.cpp \
	parser/html/tokenizer/bogus_doctype.hpp \
	parser/html/context.hpp \
	parser/html/token.hpp \
	parser/html/tokenizer.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tokenizer/bogus_doctype.cpp

bin/parser/html/tokenizer/character_reference.o: parser/html/tokenizer/character_reference.cpp \
	parser/html/tokenizer/character_reference.hpp \
	parser/html/context.hpp \
	parser/html/token.hpp \
	parser/html/tokenizer.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tokenizer/character_reference.cpp

bin/parser/html/tokenizer/comment_end_bang.o: parser/html/tokenizer/comment_end_bang.cpp \
	parser/html/tokenizer/comment_end_bang.hpp \
	parser/html/context.hpp \
	parser/html/token.hpp \
	parser/html/tokenizer.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tokenizer/comment_end_bang.cpp

bin/parser/html/tokenizer/comment_end_dash.o: parser/html/tokenizer/comment_end_dash.cpp \
	parser/html/tokenizer/comment_end_dash.hpp \
	parser/html/context.hpp \
	parser/html/token.hpp \
	parser/html/tokenizer.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tokenizer/comment_end_dash.cpp

bin/parser/html/tokenizer/comment_end.o: parser/html/tokenizer/comment_end.cpp \
	parser/html/tokenizer/comment_end.hpp \
	parser/html/context.hpp \
	parser/html/token.hpp \
	parser/html/tokenizer.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tokenizer/comment_end.cpp

bin/parser/html/tokenizer/comment_lts_bang_dash_dash.o: parser/html/tokenizer/comment_lts_bang_dash_dash.cpp \
	parser/html/tokenizer/comment_lts_bang_dash_dash.hpp \
	parser/html/context.hpp \
	parser/html/token.hpp \
	parser/html/tokenizer.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tokenizer/comment_lts_bang_dash_dash.cpp

bin/parser/html/tokenizer/comment_lts_bang_dash.o: parser/html/tokenizer/comment_lts_bang_dash.cpp \
	parser/html/tokenizer/comment_lts_bang_dash.hpp \
	parser/html/context.hpp \
	parser/html/token.hpp \
	parser/html/tokenizer.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tokenizer/comment_lts_bang_dash.cpp

bin/parser/html/tokenizer/comment_lts_bang.o: parser/html/tokenizer/comment_lts_bang.cpp \
	parser/html/tokenizer/comment_lts_bang.hpp \
	parser/html/context.hpp \
	parser/html/token.hpp \
	parser/html/tokenizer.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tokenizer/comment_lts_bang.cpp

bin/parser/html/tokenizer/comment_lts.o: parser/html/tokenizer/comment_lts.cpp \
	parser/html/tokenizer/comment_lts.hpp \
	parser/html/context.hpp \
	parser/html/token.hpp \
	parser/html/tokenizer.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tokenizer/comment_lts.cpp

bin/parser/html/tokenizer/comment_start_dash.o: parser/html/tokenizer/comment_start_dash.cpp \
	parser/html/tokenizer/comment_start_dash.hpp \
	parser/html/context.hpp \
	parser/html/token.hpp \
	parser/html/tokenizer.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tokenizer/comment_start_dash.cpp

bin/parser/html/tokenizer/comment_start.o: parser/html/tokenizer/comment_start.cpp \
	parser/html/tokenizer/comment_start.hpp \
	parser/html/context.hpp \
	parser/html/token.hpp \
	parser/html/tokenizer.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tokenizer/comment_start.cpp

bin/parser/html/tokenizer/comment.o: parser/html/tokenizer/comment.cpp \
	parser/html/tokenizer/comment.hpp \
	parser/html/context.hpp \
	parser/html/token.hpp \
	parser/html/tokenizer.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tokenizer/comment.cpp

bin/parser/html/tokenizer/doctype_name.o: parser/html/tokenizer/doctype_name.cpp \
	parser/html/tokenizer/doctype_name.hpp \
	parser/html/context.hpp \
	parser/html/token.hpp \
	parser/html/tokenizer.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tokenizer/doctype_name.cpp

bin/parser/html/tokenizer/doctype_public_identifier_dq.o: parser/html/tokenizer/doctype_public_identifier_dq.cpp \
	parser/html/tokenizer/doctype_public_identifier_dq.hpp \
	parser/html/context.hpp \
	parser/html/token.hpp \
	parser/html/tokenizer.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tokenizer/doctype_public_identifier_dq.cpp

bin/parser/html/tokenizer/doctype_public_identifier_sq.o: parser/html/tokenizer/doctype_public_identifier_sq.cpp \
	parser/html/tokenizer/doctype_public_identifier_sq.hpp \
	parser/html/context.hpp \
	parser/html/token.hpp \
	parser/html/tokenizer.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tokenizer/doctype_public_identifier_sq.cpp

bin/parser/html/tokenizer/doctype_system_identifier_dq.o: parser/html/tokenizer/doctype_system_identifier_dq.cpp \
	parser/html/tokenizer/doctype_system_identifier_dq.hpp \
	parser/html/context.hpp \
	parser/html/token.hpp \
	parser/html/tokenizer.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tokenizer/doctype_system_identifier_dq.cpp

bin/parser/html/tokenizer/doctype_system_identifier_sq.o: parser/html/tokenizer/doctype_system_identifier_sq.cpp \
	parser/html/tokenizer/doctype_system_identifier_sq.hpp \
	parser/html/context.hpp \
	parser/html/token.hpp \
	parser/html/tokenizer.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tokenizer/doctype_system_identifier_sq.cpp

bin/parser/html/tokenizer/doctype.o: parser/html/tokenizer/doctype.cpp \
	parser/html/tokenizer/doctype.hpp \
	parser/html/context.hpp \
	parser/html/token.hpp \
	parser/html/tokenizer.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tokenizer/doctype.cpp

bin/parser/html/tokenizer/data.o: parser/html/tokenizer/data.cpp \
	parser/html/tokenizer/data.hpp \
	parser/html/context.hpp \
	parser/html/token.hpp \
	parser/html/tokenizer.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tokenizer/data.cpp

bin/parser/html/tokenizer/markup_declaration_open.o: parser/html/tokenizer/markup_declaration_open.cpp \
	parser/html/tokenizer/markup_declaration_open.hpp \
	parser/html/context.hpp \
	parser/html/token.hpp \
	parser/html/tokenizer.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tokenizer/markup_declaration_open.cpp

bin/parser/html/tokenizer/self_closing_start.o: parser/html/tokenizer/self_closing_start.cpp \
	parser/html/tokenizer/self_closing_start.hpp \
	parser/html/context.hpp \
	parser/html/token.hpp \
	parser/html/tokenizer.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tokenizer/self_closing_start.cpp

bin/parser/html/tokenizer/tag_end_open.o: parser/html/tokenizer/tag_end_open.cpp \
	parser/html/tokenizer/tag_end_open.hpp \
	parser/html/context.hpp \
	parser/html/token.hpp \
	parser/html/tokenizer.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tokenizer/tag_end_open.cpp

bin/parser/html/tokenizer/tag_name.o: parser/html/tokenizer/tag_name.cpp \
	parser/html/tokenizer/tag_name.hpp \
	parser/html/context.hpp \
	parser/html/token.hpp \
	parser/html/tokenizer.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tokenizer/tag_name.cpp

bin/parser/html/tokenizer/tag_open.o: parser/html/tokenizer/tag_open.cpp \
	parser/html/tokenizer/tag_open.hpp \
	parser/html/context.hpp \
	parser/html/token.hpp \
	parser/html/tokenizer.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tokenizer/tag_open.cpp

bin/parser/html/tree/insert_before_head.o: parser/html/tree/insert_before_head.cpp \
	parser/html/tree/insert_before_head.hpp \
	dom/comment.hpp \
	dom/element.hpp \
	parser/html/constants.hpp \
	parser/html/tree/insertion_mode.hpp \
	logger.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tree/insert_before_head.cpp

bin/parser/html/tree/insert_before_html.o: parser/html/tree/insert_before_html.cpp \
	parser/html/tree/insert_before_html.hpp \
	dom/comment.hpp \
	dom/element.hpp \
	parser/html/constants.hpp \
	parser/html/tree/insertion_mode.hpp \
	logger.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tree/insert_before_html.cpp

bin/parser/html/tree/insert_in_head.o: parser/html/tree/insert_in_head.cpp \
	parser/html/tree/insert_in_head.hpp \
	dom/comment.hpp \
	dom/element.hpp \
	parser/html/constants.hpp \
	parser/html/tree/insertion_mode.hpp \
	logger.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tree/insert_in_head.cpp

bin/parser/html/tree/insert_intial.o: parser/html/tree/insert_initial.cpp \
	parser/html/tree/insert_initial.hpp \
	dom/comment.hpp \
	dom/element.hpp \
	parser/html/constants.hpp \
	parser/html/tree/insertion_mode.hpp \
	logger.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ parser/html/tree/insert_initial.cpp

bin/rendering/opengl/gl_renderer.o: rendering/opengl/gl_renderer.cpp \
	rendering/opengl/gl_renderer.hpp \
	rendering/renderer.hpp \
	rendering/drawables/draw_rect.hpp \
	logger.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ rendering/opengl/gl_renderer.cpp

bin/rendering/window/window_glfw.o: rendering/window/window_glfw.cpp \
	rendering/window/window_glfw.hpp \
	rendering/window/window.hpp \
	logger.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ rendering/window/window_glfw.cpp

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
# statically analyzes the code, and might catch bugs and suggest improvements.
cppcheck:
	cppcheck -I. -q --verbose --std=c++17 --enable=all .

# the 'infer' target will invoke the Infer program, so Infer is prerequisite.
# Infer will analyze the software for common bugs.
infer:
	infer run -- make

# the 'infer-clean' target will invoke the Infer program and makes sure older
# binaries will be cleaned up first, so that the complete software will be
# analyzed. Running infer again doesn't require a clean build, so use the
# 'infer' target after running the 'infer-clean' target once.
infer-clean:
	infer run -- make clean all

# A modern linter for C++, made by Facebook.
flint:
	flint++ -r -v .
