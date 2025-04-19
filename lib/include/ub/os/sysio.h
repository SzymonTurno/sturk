#ifndef UB_SYSIO_H
#define UB_SYSIO_H

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct UBfstream UBfstream;

bool ub_fexists(const char* filename);

UBfstream* ub_fopen(const char *filename, const char *mode);

void ub_fclose(UBfstream* stream);

int ub_fgetc(UBfstream* stream);

char* ub_fgets(char* str, int size, UBfstream* stream);

int ub_fputs(const char* str, UBfstream* stream);

int ub_fseekset(UBfstream* stream, long int offset);

int ub_fseekcur(UBfstream* stream, long int offset);

int ub_fseekend(UBfstream* stream, long int offset);

UBfstream* ub_stdout(void);

UBfstream* ub_stderr(void);

int ub_fvprintf(UBfstream* stream, const char* format, va_list vlist);

int ub_snprintf(char* buffer, size_t bufsz, const char* format, ...);

int ub_vsnprintf(char* buffer, size_t bufsz, const char* format, va_list vlist);

char* ub_getcwd(char* buf, size_t size);

void ub_sysfail(void);

#endif /* UB_SYSIO_H */
