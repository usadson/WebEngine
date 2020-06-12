# Code Style
This code of this project mainly consists of C/C++17 code.
The code style closest to the code in this project is the [OpenBSD style](https://man.openbsd.org/style).

## Include rules
`#include`s are formatted in form (each section is seperated by an empty line):
* _If this is a .cpp file:_ `#include "<name>.hpp"`
* `#include` C++ STL Files
* `#include` C STL Files
* `#include` Libraries
* `#include` Header Files

## Indentation
Identation are tabulators.
Unfortunately, due to namespace's and C++'s long names, a 80-character line-limit is impossible,
but try to conform to that rule when writing comments, and seperating the argument list by newlines
to prevent long-lines is good practice.

## Clang-format
For the most part, use `clang-format` to format the code. The configuration of this tool still needs some tweaking, but in general it works.

* Template issues:
```cpp
std::optional<std::vector<std::string>>
```
will be converted to
```cpp
std::optional<std::vector<std::string> >
```
* class keywords (public, protected, private) use two-space indentation instead of tabs.
