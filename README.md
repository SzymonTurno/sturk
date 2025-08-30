# Sturk

<!--![TOC]!-->


[![License](https://img.shields.io/badge/License-BSD_3--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)


[Sturk](https://szymonturno.github.io/sturk/)
is a C library that implements the publish-subscribe messaging pattern.


## Portability

- C99 support: `gcc -std=c99 -pedantic ...`.
- CI/CD platforms: ubuntu, macos, windows.
- OSAL.


## Usage

Sturk is designed with an intention to be used as a static library.
Therefore, the most straightforward approach to compile a program that uses Sturk would
be to build in two steps:

1. compile Sturk library,
2. compile the program while linking to Sturk library and its header files.


> [!note]
> - By default, the library is built at `/path/to/sturk/build/src/libsturk.a`.
> - Header files are located at `/path/to/sturk/include`.


**Compile `libsturk.a`**

```sh
cd /path/to/sturk
make
```


**Link to `libsturk.a`**

```sh
cc main.c /path/to/sturk/build/src/libsturk.a -I/path/to/sturk/include
```


### Configuring

See [Buildsystem](#buildsystem).



## Architecture

| Module                               | Provides                                   |
| ------------------------------------ | ------------------------------------------ |
| [broker](src/broker/README.md)       | publish-subscribe utility                  |
| algo                                 | memory pool; waiting queue; red-black tree |
| logger                               | logging to stdout/stderr/file              |
| osal                                 | memory allocator; filesystem; thread sync. |
| basis                                | arena                                      |
| [vertegs](include/vertegs/README.md) | subroutines for operating on graphs        |


| ![](http://www.plantuml.com/plantuml/proxy?cache=no&src=https://raw.githubusercontent.com/SzymonTurno/sturk/refs/heads/feat/arena-allocator/docs/hierarchy.puml) |
| :---------------: |
| Modules hierarchy |


## Buildsystem<!--!{#buildsystem}!-->

WIP

The build can be customized with a yaml configuration.
In this case, `libsturk.a` will be created at `/my/empty/dir`.

```sh
cd /my/empty/dir
/path/to/sturk/tools/configure.sh /path/to/config.yaml
make
```


## Glossary

| Term | Description                        |
| ---- | ---------------------------------- |
| OSAL | operating system abstraction layer |
