# Building
## Toolchain
The following toolchain is used for compilation and linking:
### Clang
This project uses the modern [Clang](https://clang.llvm.org/) compiler.
This compiler is aiming as a drop-in replacement for GCC, so if you have experience with that compiler, you'll probably feel comfortable with the tool.
The project is maintained by various respected companies, like Apple, ARM, Google, Mozilla, Sony etc.

### Make
The project uses [Makefile](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/make.html)s as the automatic building tool.

### pkg-config
The project uses [pkg-config](https://www.freedesktop.org/wiki/Software/pkg-config/) for finding and linking dependencies. It is by default installed on most Linux distributions, but is available in most *nix package managers (like homebrew, pkg, etc.).

## Dependencies
This project depends on various other open-source libraries.
### LibreSSL
[LibreSSL](https://libressl.org/) is used as the TLS stack library. It is a fork of [OpenSSL](https://openssl.org), and aims to provide a more secure API through various sub-libraries, like `libtls`.

Even though LibreSSL is used by default, it is made possible to change the library by implementing a different source-code implementation of the TLS tools in [net/connection_info.hpp](../net/connection_info.hpp). LibreSSL is implemented as [net/connection_info_libtls.hpp](../net/connection_info_libtls.hpp).

Click [here](https://www.libressl.org/releases.html) for instructions to install LibreSSL.
### GLEW
The renderer stack uses OpenGL by default as its rendering back-end, and to use modern OpenGL (GL 2.0+), [GLEW](http://glew.sourceforge.net/) is used.

Click [here](http://glew.sourceforge.net/index.html) for binary releases, or building instructions.

### GLFW
To interact with most window managers, [GLFW](https://www.glfw.org/) is used.
Click [here](https://www.glfw.org/download.html) for binary releases and building instructions.

### FreeType
[FreeType](https://freetype.org/) is used for drawing text. Click [here](https://freetype.org/download.html) for binary releases and building instructions.
### GoogleTest
To automatically test various components of this project, unit tests have been built using [GoogleTest](https://github.com/google/googletest/).
This tool isn't necessary required for building, but is still recommended.
### Getting dependencies on Debian-like systems (Debian, Ubuntu, Kubuntu, etc.)
This section is for ease-of-use, and may possibly not work 100%. Please refer to the project's website for proper instructions.
#### LibreSSL
Use the following commands to download, build and install LibreSSL:
```sh
git clone https://github.com/libressl-portable/portable
cd portable
./config
make all test
sudo make install
```
### GoogleTest
```sh
sudo apt install googletest libgtest-dev && sudo apt install googletest-tools
```
GoogleTest may not be included in all distro's, so if you get an error, run the following commands as well:
```sh
wget https://github.com/google/googletest/archive/v1.10.x.tar.gz
tar -xf v1.10.x.tar.gz
cd googletest-1.10.x
cmake .
make
sudo make install
cd ..
```
### Other tools
Use the following commands to install the other dependencies:
```sh
sudo apt update -y
sudo apt -y install libglew-dev libglfw3-dev
sudo apt -y install libfreetype-dev || sudo apt -y install libfreetype6-dev
```

## Building
With all the dependencies setup, building the project is quite simple:
```sh
make
```
The following tool will run the make recipes single-threadedly. Use the following tool for fast compilation:
```sh
tools/build-fast.sh
```
