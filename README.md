# Sturk

<!--![TOC]!-->


[![License](https://img.shields.io/badge/License-BSD_3--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)


[Sturk](https://szymonturno.github.io/sturk/)
is a C library that implements the publish-subscribe messaging pattern.


## Features

- User-defined API for constructing messages.
- Red-black tree for the channels dictionary.
- Memory pools for messages and queues.
- Logger with trace levels and exceptions.
- Common data structures: list, queue, dictionary.
- Single-thread implementation.
- Multi-thread implementation.


## Portability

- C99 support: `gcc -std=c99 -pedantic ...`.
- CI/CD platforms: ubuntu, macos, windows.
- Operating system abstraction layer.


## Build

Sources are compiled into a static library - `libsturk.a`.
The library uses header files from the directory `/path/to/sturk/include`.

```sh
-I/path/to/sturk/include
```


By default, `libsturk.a` will be created at `/path/to/sturk/build/src`.

```sh
cd /path/to/sturk
make
```


The build can be customized with a yaml configuration.
In this case, `libsturk.a` will be created at `/my/empty/dir`.

```sh
cd /my/empty/dir
/path/to/sturk/tools/configure.sh /path/to/config.yaml
make
```

---


## --- USAGE ---

- [Message broker](src/broker/README.md)
- [Graphs library](include/vertegs/README.md)
