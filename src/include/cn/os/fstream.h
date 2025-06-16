#ifndef CN_FSTREAM_H
#define CN_FSTREAM_H

#include <stdarg.h>
#include <stddef.h>

typedef struct CnFstream CnFstream;

CnFstream* cn_fopen(const char* filename, const char* mode);

void cn_fclose(CnFstream* stream);

char* cn_fgets(char* str, int size, CnFstream* stream);

int cn_fseekset(CnFstream* stream, long int offset);

CnFstream* cn_stdout(void);

CnFstream* cn_stderr(void);

int cn_fvprintf(CnFstream* stream, const char* format, va_list vlist);

#endif /* CN_FSTREAM_H */
