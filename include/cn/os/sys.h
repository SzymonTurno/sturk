#ifndef CN_SYS_H
#define CN_SYS_H

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

int cn_snprintf(char* buffer, size_t bufsz, const char* format, ...);

void cn_sysfail(void);

int cn_remove(const char* filename);

#endif /* CN_SYS_H */
