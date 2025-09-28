# Sturk

<!--![TOC]!-->

[![License](https://img.shields.io/badge/License-BSD_3--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)

[Sturk](https://szymonturno.github.io/sturk/)
is a C library that implements the publish-subscribe messaging pattern.

## Quickstart

See [publish-subscribe](#publish-subscribe).

## Portability

- C99 support: `gcc -std=c99 -pedantic ...`.
- CI/CD platforms: ubuntu, macos, windows.
- OSAL.

## Usage

Sturk is designed with an intention to be used as a static library.
Therefore, the most straightforward approach to compile a program that uses
Sturk would be to build in two steps:

1. compile Sturk library,
2. compile the program while linking to Sturk library and its header files.

> [!note]
> - By default, the library is built at `/path/to/sturk/build/src/libsturk.a`.
> - Header files are located at `/path/to/sturk/include`.

### Compile libsturk.a

```sh
cd /path/to/sturk
make
```

### Link to libsturk.a

```sh
cc main.c /path/to/sturk/build/src/libsturk.a -I/path/to/sturk/include
```

### Configuring

To configure the build see [buildsystem](#buildsystem).

To choose an OS or to add support for an OS see also [osal](src/osal/README.md).

> [!note]
> By default multithreading is not supported.

## Overview

Sturk consists of several modules with dependencies as shown in the
**Modules hierarchy** diagram.

| ![modules-hierarchy](http://www.plantuml.com/plantuml/proxy?cache=no&src=https://raw.githubusercontent.com/SzymonTurno/sturk/refs/heads/main/docs/hierarchy.puml) |
| :---------------: |
| Modules hierarchy |

The role of each module, hyperlink to its documentation and the location of its
public header files are gathered in the **Modules** table. The ordering of the
table corresponds to the modules hierarchy.

Table: Modules

|  #  | Module                               | Provides                                   | Include subdirectory         |
| --- | ------------------------------------ | ------------------------------------------ | ---------------------------- |
|  1  | [broker](src/broker/README.md)       | publish-subscribe utility                  | `st` (`sturk`)               |
|  2  | [algo](src/algo/README.md)           | memory pool; waiting queue; dictionary     | `st` (`sturk`)               |
|  3  | [logger](src/logger/README.md)       | logging to stdout/stderr/file              | `st/logger` (`sturk/logger`) |
|  4  | [osal](src/osal/README.md)           | memory allocator; filesystem; thread sync. | `st/os` (`sturk/os`)         |
|  5  | [basis](src/basis/README.md)         | arena                                      | `st` (`sturk`)               |
|  6  | [vertegs](include/vertegs/README.md) | subroutines for operating on graphs        | `vertegs`                    |

## Buildsystem<!--!{#buildsystem}!-->

The build is configured with a single yaml file. Before running make the configuration
file must be passed to the [configure.sh](https://github.com/SzymonTurno/sturk/blob/main/tools/configure.sh)
script in order to generate a makefile.

```sh
mkdir build
cd build

# generate makefile
/path/to/sturk/tools/configure.sh /path/to/config.yaml

# build default target - libsturk.a
make
```

All possible build variables can be found inside `olconf.yaml` files from the `src`
directory. The configuration is processed by `olconf.py` scripts.

### C99 support

Configuring with `cver: "gnu"` will pass `-std=gnu11` to the compiler while
`cver: "iso"` will pass `-std=c99 -pedantic` instead.

See [src/olconf.py](https://github.com/SzymonTurno/sturk/blob/main/src/olconf.py).

```python
    if settings['cver'] == 'gnu':
        olvars.append('sturk_EXTRA_CFLAGS', '-std=gnu11')
    elif settings['cver'] == 'iso':
        olvars.append('sturk_EXTRA_CFLAGS', '-std=c99')
        olvars.append('sturk_EXTRA_CFLAGS', '-pedantic')
    else:
        olvars.fail('Unknown cver: ' + settings['cver'] + '.')
```

### Example configuration

As an example see [configs/iso/olconf.yaml](https://github.com/SzymonTurno/sturk/blob/main/configs/iso/olconf.yaml)
and compare it to [src/olconf.yaml](https://github.com/SzymonTurno/sturk/blob/main/src/olconf.yaml).

The following

```sh
/path/to/sturk/tools/configure.sh /path/to/sturk/configs/iso/olconf.yaml
```

would overwrite the default configuration from `src/olconf.yaml`:

- `cver: "gnu"`,
- `build_type: "release"`;

with the configuration from `configs/iso/olconf.yaml`:

- `cver: "iso"`,
- `build_type: "coverage"`.

## Glossary

Table: Glossary

| Term | Description                                |
| ---- | ------------------------------------------ |
| OSAL | Operating System Abstraction Layer         |
| API  | Application Programming Interface          |
| cirq | short name for doubly linked circular list |
