# OSAL

<!--![TOC]!-->

Operating System Abstraction Layer (OSAL) gathers all os-specific operations and
provides a common API for all implementations. It consists of four submodules
configured with the `osal` section from the yaml configuration file - see
[default osal configuration](https://github.com/SzymonTurno/sturk/blob/main/src/osal/olconf.yaml).

Table: OSAL

|  #  | API                                                            | Implementations       |
| --- | -------------------------------------------------------------- | --------------------- |
|  1  | [mem](https://szymonturno.github.io/sturk/os_2mem_8h.html)     | `arena`, `ext`, `std` |
|  2  | [mutex](https://szymonturno.github.io/sturk/os_2mutex_8h.html) | `none`, `posix`       |
|  3  | [sem](https://szymonturno.github.io/sturk/os_2sem_8h.html)     | `none`, `posix`       |
|  4  | [sys](https://szymonturno.github.io/sturk/os_2sys_8h.html)     | `none`, `std`         |

## Configuring OSAL

For the following configuration file

```yaml
---
  osal:
    <api>: "<implementation>"
```

the source file `/path/to/sturk/src/osal/<api>/<implementation>.c` will be added
to the build.
