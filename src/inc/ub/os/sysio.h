#ifndef UB_SYSIO_H
#define UB_SYSIO_H

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct UBfstream UBfstream;

bool ub_fs_fexists(const char* filename);

UBfstream* ub_fs_fopen(const char *filename, const char *mode);

void ub_fs_fclose(UBfstream* stream);

int ub_fs_fgetc(UBfstream* stream);

char* ub_fs_fgets(char* str, int size, UBfstream* stream);

int ub_fs_fseekcur(UBfstream* stream, long int offset);

UBfstream* ub_fs_stdout(void);

UBfstream* ub_fs_stderr(void);

int ub_fs_vprintf(UBfstream* stream, const char* format, va_list vlist);

int ub_snprintf(char* buffer, size_t bufsz, const char* format, ...);

int ub_vsnprintf(char* buffer, size_t bufsz, const char* format, va_list vlist);

char* ub_getcwd(char* buf, size_t size);

#endif /* UB_SYSIO_H */
