# WebEngine
| Travis CI | Github Actions | CodeFactor | License
|-|-|-|-
| [![Build Status](https://travis-ci.com/usadson/WebEngine.svg?branch=master)](https://travis-ci.com/usadson/WebEngine) | ![C/C++ CI](https://github.com/usadson/WebEngine/workflows/C/C++%20CI/badge.svg) | [![CodeFactor](https://www.codefactor.io/repository/github/usadson/webengine/badge)](https://www.codefactor.io/repository/github/usadson/webengine) | [![License](https://img.shields.io/badge/License-BSD%202--Clause-orange.svg)](https://opensource.org/licenses/BSD-2-Clause)

This project is the repository of WebEngine, a free-and-open-source web-browser project. This isn't a full-fledged browser — of course — and is just a hobbyist project. Feel free to look through the code and maybe even contribute to the project.

## Code Style
This code of this project mainly consists of C/C++17 code.
The code style closest to the code in this project is the [OpenBSD style](https://man.openbsd.org/style).
`#include`s are formatted in form (each section is seperated by an empty line):
* _If this is a .cpp file:_ `#include "<name>.hpp"`
* `#include` C++ STL Files
* `#include` C STL Files
* `#include` Libraries
* `#include` Header Files

Identation are tabulators.
Unfortunately, due to namespace's and C++'s long names, a 80-character line-limit is impossible,
but try to conform to that rule when writing comments, and seperating the argument list by newlines
to prevent long-lines is good practice.

For the most part, use `clang-format` to format the code. The configuration of this tool still needs some tweaking, but in general it works.
Known issues:
* Things like: `std::optional<std::vector<std::string>>` will be converted to `std::optional<std::vector<std::string> >`
* class keywords (public, protected, private) use two-space indentation instead of tabs.
