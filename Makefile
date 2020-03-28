CFLAGS = -Isrc -g -Wall -std=c++17 -O0
CXX = c++
LDFLAGS = 

BINARIES = bin/parser/html/context.so \
	   bin/parser/html/error.so \
	   bin/parser/html/token.so \
	   bin/parser/html/tokenizer.so \
	   bin/parser/html/tree_constructor.so

engine: src/main.cpp \
	src/parser/html/error.hpp \
	src/parser/html/state.hpp \
	$(BINARIES)
	$(CXX) $(CFLAGS) -o $@ src/main.cpp $(LDFLAGS) $(BINARIES)

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
	src/parser/html/tree_constructor.hpp
	$(CXX) $(CFLAGS) -c -o $@ src/parser/html/tokenizer.cpp

bin/parser/html/tree_constructor.so: src/parser/html/tree_constructor.cpp \
	src/parser/html/tree_constructor.hpp
	$(CXX) $(CFLAGS) -c -o $@ src/parser/html/tree_constructor.cpp
