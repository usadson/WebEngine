# WebEngine
| Travis CI | Github Actions | CodeFactor | License
|-|-|-|-
| [![Build Status](https://travis-ci.com/usadson/WebEngine.svg?branch=master)](https://travis-ci.com/usadson/WebEngine) | ![C/C++ CI](https://github.com/usadson/WebEngine/workflows/C/C++%20CI/badge.svg) | [![CodeFactor](https://www.codefactor.io/repository/github/usadson/webengine/badge)](https://www.codefactor.io/repository/github/usadson/webengine) | [![License](https://img.shields.io/badge/License-BSD%202--Clause-orange.svg)](https://opensource.org/licenses/BSD-2-Clause)

This project is the repository of WebEngine, a free-and-open-source web-browser project. This isn't a full-fledged browser — of course — and is just a hobbyist project. Feel free to look through the code and maybe even contribute to the project.

## Dependencies
This project uses [LibreSSL](https://libressl.org/) as its TLS library,
GLFW for the window manager class (this can be changed, and using a different implementation is possible by deriving from the [Rendering::Window](rendering/window/window.hpp).
WebEngine chooses from the supported renderers, this *will include* [OpenGL](https://www.opengl.org/),
[Vulkan](https://www.khronos.org/vulkan/), a software renderer, and maybe [DirectX](https://en.wikipedia.org/wiki/DirectX/).
For OpenGL, the project depends on [GLEW](http://glew.sourceforge.net/), an OpenGL loader.
To draw fonts, this project uses [FreeType](https://www.freetype.org/).

## Building
To build the project, make sure you have the required dependencies stated above.
To make a simple build on Linux, a call to `make` is adequate.
This will use the clang compiler and uses pkg-config to locate the dependencies.
To change the default compilation, edit the [Makefile](Makefile).

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

## Directory Structure
The project is seperated into different directories.
* [bin](bin) is the auto-generated directory for object files. Note however that the main executable is the `./engine` file.
* [data](data) is the directory of small parsers/converters.
  * [text](data/text) is the directory for the [Unicode header](data/text/unicode.hpp) and [Unicode String class](data/text/ustring.cpp).
    * [encoding](data/text/encoding) is the directory for the encodings.
* [dom](dom) is the implementation of the [Document Object Model](https://dom.spec.whatwg.org/).
* [misc](misc) is the directory for small stuff not related to any big subjects.
* [net](net) is the network layer. This uses — at the moment — [POSIX/UNIX sockets](https://man.openbsd.org/socket.2) and an abstract TLS implementation (this is currently set to use the [libtls/LibreSSL](https://libressl.org/) library).
  * [http](net/http) is the [HTTP/1.1](https://www.rfc-editor.org/rfc/rfc7230.html) network layer implementation, which is used to retrieve documents and resources.
* [parser](parser) is the directory for major parsers (see [data](data) for small parser/converters).
  * [html](parser/html) is the directory for the HTML parser, which should follow the [HTML Living Standard](https://html.spec.whatwg.org/multipage/).
    * [tokenizer](parser/html/tokenizer) are the modularized parts of the HTML tokenizer.
    * [tree](parser/html/tree) is the directory for the [HTML Tree Constructor](https://html.spec.whatwg.org/multipage/parsing.html#tree-construction).
* [rendering](rendering) is the directory for rendering/painting the UI, webpage, etc.
  * [drawables](rendering/drawables) is the directory for all things that derive from [RenderObject](rendering/render_object.hpp), which is a part of the renderer to draw. This *will include* text, rects, assets (images, videos, audio, etc.)
  * [opengl](rendering/opengl) is the [OpenGL](https://opengl.org/) implementation of the [Renderer class](rendering/renderer.hpp).
  * [window](rendering/window) is the directory containing window system/manager communicators. These are seperate from the renderer to avoid code duplication between platforms.
* [resources](resources) contains files which are pulled from the [internet](net) or from local storage.
* [testing](testing) contains GoogleTest files
* [tools](tools) contains various custom made tools for this project

## License
The code of this project is governed under the [BSD 2-Clause](https://choosealicense.com/licenses/bsd-2-clause/) license.
See the [COPYING](COPYING) file for legal text.
This project uses parts of GLFW, which is governed under the [zlib/libpng license](https://www.glfw.org/license.html),
parts of GLEW largely under the [modified BSD/MIT license](https://github.com/nigels-com/glew/blob/master/LICENSE.txt),
parts of FreeType which is licensed under the [permissive FTL license](https://git.savannah.gnu.org/cgit/freetype/freetype2.git/plain/docs/FTL.TXT),
and LibreSSL, which is licensed under [various licenses](https://github.com/libressl/libressl/blob/master/src/LICENSE).

You can also view licensing information by supplying the `--credits` option to the binary executable.
