##Directory Structure
The project is seperated into different directories. This document explains what each folder contains.

* `bin` is the auto-generated directory for object files. Note however that the main executable is the `./engine` file.
* [data](../data) is the directory of small parsers/converters.
  * [text](../data/text) is the directory for the [Unicode header](../data/text/unicode.hpp) and [Unicode String class](../data/text/ustring.cpp).
    * [encoding](../data/text/encoding) is the directory for the encodings.
* [docs](../docs)
* [dom](../dom) is the implementation of the [Document Object Model](https://dom.spec.whatwg.org/).
* [misc](../misc) is the directory for small stuff not related to any big subjects.
* [net](../net) is the network layer. This uses — at the moment — [POSIX/UNIX sockets](https://man.openbsd.org/socket.2) and an abstract TLS implementation (this is currently set to use the [libtls/LibreSSL](https://libressl.org/) library).
  * [http](../net/http) is the [HTTP/1.1](https://www.rfc-editor.org/rfc/rfc7230.html) network layer implementation, which is used to retrieve documents and resources.
* [parser](../parser) is the directory for major parsers (see [data](data) for small parser/converters).
  * [html](../parser/html) is the directory for the HTML parser, which should follow the [HTML Living Standard](https://html.spec.whatwg.org/multipage/).
    * [tokenizer](../parser/html/tokenizer) are the modularized parts of the HTML tokenizer.
    * [tree](../parser/html/tree) is the directory for the [HTML Tree Constructor](https://html.spec.whatwg.org/multipage/parsing.html#tree-construction).
* [rendering](../rendering) is the directory for rendering/painting the UI, webpage, etc.
  * [drawables](../rendering/drawables) is the directory for all things that derive from [RenderObject](../rendering/render_object.hpp), which is a part of the renderer to draw. This *will include* text, rects, assets (images, videos, audio, etc.)
  * [opengl](../rendering/opengl) is the [OpenGL](https://opengl.org/) implementation of the [Renderer class](../rendering/renderer.hpp).
  * [window](../rendering/window) is the directory containing window system/manager communicators. These are seperate from the renderer to avoid code duplication between platforms.
* [resources](../resources) contains files which are pulled from the [internet](../net) or from local storage.
* [testing](../testing) contains GoogleTest files
* [tools](../tools) contains various custom made tools for this project
