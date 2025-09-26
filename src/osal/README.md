# OSAL

<!--![TOC]!-->

Operating System Abstraction Layer (OSAL) gathers all os-specific operations and
provides a common API for all implementations. It consists of five submodules
configured with the `osal` section from the yaml configuration file - see
[default osal configuration](https://github.com/SzymonTurno/sturk/blob/main/src/osal/olconf.yaml).

Table: OSAL

|  #  | API                                                            | Implementations |
| --- | -------------------------------------------------------------- | --------------- |
|  1  | [fstream](https://szymonturno.github.io/sturk/fstream_8h.html) | `std`           |
|  2  | [mem](https://szymonturno.github.io/sturk/os_2mem_8h.html)     | `arena`, `std`  |
|  3  | [mutex](https://szymonturno.github.io/sturk/os_2mutex_8h.html) | `none`, `posix` |
|  4  | [sem](https://szymonturno.github.io/sturk/os_2sem_8h.html)     | `none`, `posix` |
|  5  | [sys](https://szymonturno.github.io/sturk/sys_8h.html)         | `std`           |

## Configuring OSAL

For a given configuration file

```yaml
---
  osal:
    <api>: "<implementation>"
```

a source file `/path/to/sturk/src/osal/<api>/<implementation>.c` will be added
to the build.
