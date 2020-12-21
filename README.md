# Atlas
Cross-platform NES emulator written in C++.

## Getting started
This project is built using [Bazel.](https://docs.bazel.build/versions/3.1.0/install.html) 

**Build**
```bash
# On Linux
$ sudo apt install libgl1-mesa-dev xorg-dev cmake

# On Mac
$ sudo port install cmake
$ ln -s /opt/local/bin/cmake /usr/local/bin/cmake # bazel does not look in opt/
```

**Run**
```bash
$ bazel run atlas
```

## Run the linter

**Install with Macports**
```bash
$ sudo port install clang-9.0
$ sudo port select --set clang mp-clang-9.0
```

**Lint the source files**
```bash
$ git-clang-format-mp-9.0
```

## Run the benchmarks

```
$ bazel run benchmark
```
