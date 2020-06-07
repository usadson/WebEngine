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
TESTING_LDFLAGS = `pkg-config --static --libs gtest` bin/logger.o

TESTING_TARGETS = \
	bin/testing/directory.txt \
	bin/testing/data/text/encoding/utf8

bin/testing/directory.txt:
	@mkdir bin/testing
	@mkdir bin/testing/data
	@mkdir bin/testing/data/text
	@mkdir bin/testing/data/text/encoding
	@touch bin/testing/directory.txt

# The 'engine' target will build the final binary executable.
bin/testing/data/text/encoding/utf8: testing/data/text/encoding/utf8.cpp \
	data/text/encoding/utf8.hpp
	$(CXX) $(TESTING_CXXFLAGS) -o $@ testing/data/text/encoding/utf8.cpp bin/data/text/encoding/utf8.o $(TESTING_LDFLAGS)
