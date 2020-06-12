# Testing
This project uses a lot of tools and tests to ensure and enforce the best code quality and squash bugs. This file lists the tools used within this project, and describes how to use them.

## Table of Contents
1. [Automatic testing](#automatic-testing)
2. [Run-time tools](#runtime-tools)
   1. [Valgrind](#runtime-tools-valgrind)
3. [Static Analyzers](#static-analyzers)
   1. [Clang-tidy](#static-analyzers-clang-tidy)
   2. [Cppcheck](#static-analyzers-cppcheck)
   3. [Infer](#static-analyzers-infer)
   4. [Flint++](#static-analyzers-flint)
4. [Other tools](#other-tools)
   1. [Clang-format](#other-tools-clang-format)

## Automatic testing <a name="automatic-testing"></a>
Whenever a changelog is commited and pushed, [Travis CI](https://travis-ci.com/github/usadson/WebEngine) and [GitHub Actions](https://github.com/usadson/WebEngine/actions) will initiate a fresh build each time, so that breaking changes will be catched and resolved. These make use of [BuildCache](https://github.com/usadson/BuildCache), which contains some binary files to massively increase the build speeds, which is essential, because **tests should be fast**.

[CodeFactor](https://www.codefactor.io/repository/github/usadson/webengine) is also used to analyze the code, mainly for styling issues. This process is also fully automatic, and an A+ score is aimed for.

## Run-time tools <a name="runtime-tools"></a>
Use these tools for debugging the binary executable for finding run-time bugs.
### Valgrind <a name="runtime-tools-valgrind"></a>
Even though modern C++ is used, a memory bug can still occur. [Valgrind](https://www.valgrind.org/) runs executables in a controlled environment, which exposes memory leaks, undefined behavior and other memory errors. Click [here](https://www.valgrind.org/downloads/current.html) for source code tarballs with instructions.

To install Valgrind on Debian/Ubuntu:
```sh
$ sudo apt install -y valgrind
```

A Makefile target is also included for running Valgrind with recommended settings:
```sh
$ make memory
```

## Static Analyzers <a name="static-analyzers"></a>
Use the following tools to analyze the source code for possible bugs and recommendations for best practices. Multiple static analyzers are used to ensure no problems are missed.
### Clang-tidy <a name="static-analyzers-clang-tidy"></a>
Clang-tidy is a tool made by the LLVM development team and is part of the [LLVM Project](https://llvm.org/) and [Clang Tool Suite](https://clang.llvm.org/docs/ClangTools.html). Clang is the modern compiler we use, and is made & supported by respected people, like Apple, ARM, Google, Mozilla, Sony etc.

Clang-tidy is a clever analyzer which makes sure modern C++ is used, copy reduction is at its highest, best practices are followed, etc. To install the tool, visit the [website](https://releases.llvm.org/download.html) or install it using your package manager.

There isn't an easy command to use `clang-tidy` yet on multiple files, but you can use the following command to run it on a single file:
```sh
$ clang-tidy <filename> -- -I. -std=c++17
```
Some projects and companies have included their own set of rules and recommendations, which often are non-applicable, duplicate, or unnecessary. You can omit certain tools by adding filters to the .clang-tidy configuration file.
### Cppcheck <a name="static-analyzers-cppcheck"></a>
Cppcheck is an open-source tool for finding bugs by looking at the source code. Visit the [website](http://cppcheck.sourceforge.net/) for download instructions. A Makefile target is included for running `cppcheck` with recommended settings:
```sh
$ make cppcheck
```
This will output details about the analysis, but some of the output is ignored because it contains some false positives.
### Infer <a name="static-analyzers-infer"></a>
Infer is another open-source analyzer developed by Facebook. To install the tool, [binary builds](https://fbinfer.com/docs/getting-started) are recommended for people not experienced with the Opam/OCaml tooling environment, but you can still [build it yourself](https://github.com/facebook/infer).

To run Infer, use the following Makefile target:
```sh
$ make infer
```
This will run Infer on every file that has been changed, but if you want to scan a clean build (recommended), use the following target:
```sh
$ make infer-clean
```
Which effectively does `make clean infer`.
### Flint++ <a name="static-analyzers-flint"></a>
Flint++ is another open-source static analysis tool originally developed by [Facebook](https://github.com/facebookarchive/flint). The tool is in form of a linter, which may not cleverly analyze code as other tools, but is still found very useful. Visit the [GitHub](https://github.com/JossWhittle/FlintPlusPlus) page for more information and build instructions. Flint++ is also incorporated within the Makefile:
```sh
$ make flint
```

## Other tools <a name="other-tools"></a>
### clang-format <a name="other-tools-clang-format"></a>
To enforce styling rules throughout the source code, [clang-format](https://clang.llvm.org/docs/ClangFormatStyleOptions.html) is used. This will reformat the code wherever necessary. Note that this tool isn't applied automatically, and the tool should definitely be ran after a number of changes/commits. Visit the [website](todo) for install instructions.

To run the tool throughout the codebase, a useful command is made:
```sh
$ tools/formatter.sh
```
This will run clang-format using the rules of [`.clang-format`](https://github.com/usadson/WebEngine/blob/master/.clang-format) for all .hpp and .cpp files.
