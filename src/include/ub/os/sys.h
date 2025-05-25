#ifndef UB_SYS_H
#define UB_SYS_H

#include <stdarg.h>
#include <stddef.h>

int ub_snprintf(char* buffer, size_t bufsz, const char* format, ...);

int ub_vsnprintf(char* buffer, size_t bufsz, const char* format, va_list vlist);

char* ub_getcwd(char* buf, size_t size);

void ub_sysfail(void);

int ub_remove(const char* name);

#endif /* UB_SYS_H */
