# WebEngine
This project is the repository of WebEngine (a.k.a. W4), a free-and-open-source web-browser project by me [Tristan](https://github.com/usadson). This isn't a full-fledged browser — of course — and is just a hobbyist project. Feel free to look through the code and contribute to the project.

## Dependencies
This project is using [LibreSSL](https://libressl.org/) for its TLS library, GLFW for the window manager (this can be changed, and using a different implementation is possible by deriving from the [Rendering::Window](src/rendering/window/window.hpp) class. W4 chooses from the supported renderers, this *will include* [OpenGL](https://www.opengl.org/), [Vulkan](https://www.khronos.org/vulkan/), a software renderer, and maybe [DirectX](https://en.wikipedia.org/wiki/DirectX/). For OpenGL, the project depends on [GLEW](http://glew.sourceforge.net/), an OpenGL loader. To draw fonts, this project uses [FreeType](https://www.freetype.org/).

## Building
To build the project, make sure you have the required dependencies stated above. To make a simple build on GNU/Linux, a call to `make` is adequate. This will use the `c++` command to build the project which is often symlinked to `g++`  and uses pkg-config to locate the dependencies. To change the default compilation, edit the [Makefile](Makefile). (In the future, a `./configure` system may be useful.)

## Code Style
This code of this project mainly consists of C/C++17 code. The code style closest to the code in this project is the [OpenBSD style](https://man.openbsd.org/style). `#include`s are formatted in form (each section is seperated by an empty line):
* _If this is a .cpp file:_ `#include "<name>.hpp"`
* `#include` C++ STL Files
* `#include` C STL Files
* `#include` Libraries
* `#include` Header Files

Identation are tabulators. Unfortunately, due to namespace's and C++'s long names, a 80-character line-limit is impossible, but try to conform to that rule when writing comments, and seperating the argument list by newlines to prevent long-lines is good practice.

## Directory Structure
The project is seperated into different directories, and the main source code is located in the [src](src) directory.
* [bin](bin) is the auto-generated directory for object files. Note however that the main executable is the `./engine` file.
* [src/data](src/data) is the directory of small parsers/converters.
* [src/data/text](src/data/text) is the directory for the [Unicode header](src/data/text/unicode.hpp) and [Unicode String class](src/data/text/ustring.cpp).
* [src/data/text/encoding](src/data/text/encoding) is the directory for the encodings.
* [src/dom](src/dom) is the implementation of the [Document Object Model](https://dom.spec.whatwg.org/).
* [src/net](src/net) is the network layer. This uses — at the moment — [POSIX/UNIX sockets](https://man.openbsd.org/socket.2) and an abstract TLS implementation (this is currently set to use the [libtls/LibreSSL](https://libressl.org/) library).
* [src/net/http](src/net/http) is the [HTTP/1.1](https://www.rfc-editor.org/rfc/rfc7230.html) network layer implementation, which is used to retrieve documents and resources.
* [src/parser](src/parser) is the directory for major parsers (see [src/data](src/data) for small parser/converters).
* [src/parser/html](src/parser/html) is the directory for the HTML parser, which should follow the [HTML Living Standard](https://html.spec.whatwg.org/multipage/).
* [src/parser/html/tree](src/parser/html/tree) is the directory for the [HTML Tree Constructor](https://html.spec.whatwg.org/multipage/parsing.html#tree-construction).
* [src/rendering](src/rendering) is the directory for rendering/painting the UI, webpage, etc.
* [src/rendering/drawables](src/rendering/drawables) is the directory for all things that derive from [RenderObject](src/rendering/render_object.hpp), which is a part of the renderer to draw. This *will include* text, rects, assets (images, videos, audio, etc.)
* [src/rendering/opengl](src/rendering/opengl) is the [OpenGL](https://opengl.org/) implementation of the [Renderer class](src/rendering/renderer.hpp).
* [src/rendering/window](src/rendering/window) is the directory containing window system/manager communicators. These are seperate from the renderer to avoid code duplication between platforms.
* [src/resources](src/resources) contains files which are pulled from the [internet](src/net) or from local storage.

## License
The code of this project is governed under the [ICS](https://choosealicense.com/licenses/isc/) / [MIT](https://choosealicense.com/licenses/mit/) / [BSD-2-Clause](https://choosealicense.com/licenses/bsd-2-clause/) license. See the [COPYING](COPYING) file for legal text. This project uses parts of GLFW, which is governed under the [zlib/libpng license](https://www.glfw.org/license.html), parts of GLEW under the [a modified BSD + MIT license](https://github.com/nigels-com/glew/blob/master/LICENSE.txt), parts of FreeType which is licensed under a [permissive custom license](https://git.savannah.gnu.org/cgit/freetype/freetype2.git/plain/docs/FTL.TXT), and LibreSSL, which is licensed under [various licenses](https://github.com/libressl/libressl/blob/master/src/LICENSE). This all makes redistribution in binary form possible, with mentions of this project and its dependencies.
