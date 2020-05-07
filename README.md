# AtlasEmu
Cross-platform NES emulator written in C++.

## Building
This project is built using [Bazel.](https://docs.bazel.build/versions/3.1.0/install.html) 

## Run the linter

**Install with Macports**
```bash
$ sudo port install clang-10
$ sudo port select --set clang mp-clang-10
```
**Lint the source files**
```bash
$ clang-format -i --style=file src/*.(h|cc)
```
