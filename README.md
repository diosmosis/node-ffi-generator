# node-ffi-generator

Automatically generate a JavaScript node-ffi interface by parsing C source files. Embeds the clang compiler.

## Description

This is a tool that uses clang to parse C header files, and then based on clang's AST, generates JS code that can be used with node-ffi to call and use a C library from node.js.

For example, the command:

    node-ffi-generator --file=source/my_header.h --file=source/my_dir_of_headers --src-root=source --dest-root=js-bindings --include=/usr/include --include=/path/to/clang/...

will parse my_header.h and every header in `source/my_dir_of_headers` and output JS bindings to `./js-bindings`. 

## Status

This project is still very preliminary. It works and can handle all necessary C constructs, (eg, it can parse PHP's source code), but it is still a proof of concept. The architecture has some design flaws and it is largely untested. Additionally, the build file is keyed to my dev environment. I may or may not get to fixing these problems.

## Building

These instructions are for building on linux.

1. First, download and build LLVM with clang + lld + the other dependencies needed. You can find instructions for building clang here: [http://clang.llvm.org/get_started.html](http://clang.llvm.org/get_started.html).
2. The build file is specific to my dev environment, which means you will have to change the `CMakeLists.txt` file. In the file, change all paths to LLVM so they point to where your LLVM exists.
3. Run `git submodule init` and `git submodule update`.
4. Run `mkdir -p build/Release` and `cd build/Release`.
5. Run `export CC=/path/to/clang` and `export CXX=/path/to/clang++`.
6. Run `cmake ../.. -DCMAKE_BUILD_TYPE=Release`
8. Run `make`