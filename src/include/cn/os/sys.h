#ifndef CN_SYS_H
#define CN_SYS_H

#include <stdarg.h>
#include <stddef.h>
#include <stdbool.h>

int cn_snprintf(char* buffer, size_t bufsz, const char* format, ...);

int cn_vsnprintf(char* buffer, size_t bufsz, const char* format, va_list vlist);

char* cn_getcwd(char* buf, size_t size);

void cn_sysfail(void);

bool cn_fexists(const char* filename);

int cn_remove(const char* filename);

#endif /* CN_SYS_H */
