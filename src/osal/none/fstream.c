#include "cn/os/fstream.h"
#include "osal_unistd.h"
#include <stdio.h>

CnFstream* cn_fopen(const char* filename, const char* mode)
{
	return (CnFstream*)fopen(filename, mode);
}

void cn_fclose(CnFstream* stream) { fclose((FILE*)stream); }

int cn_fgetc(CnFstream* stream) { return fgetc((FILE*)stream); }

char* cn_fgets(char* str, int size, CnFstream* stream)
{
	return fgets(str, size, (FILE*)stream);
}

int cn_fputs(const char* str, CnFstream* stream)
{
	return fputs(str, (FILE*)stream);
}

int cn_fseekset(CnFstream* stream, long int offset)
{
	return fseek((FILE*)stream, offset, SEEK_SET);
}

int cn_fseekcur(CnFstream* stream, long int offset)
{
	return fseek((FILE*)stream, offset, SEEK_CUR);
}

int cn_fseekend(CnFstream* stream, long int offset)
{
	return fseek((FILE*)stream, offset, SEEK_END);
}

CnFstream* cn_stdout(void) { return (CnFstream*)stdout; }

CnFstream* cn_stderr(void) { return (CnFstream*)stderr; }

int cn_fvprintf(CnFstream* stream, const char* format, va_list vlist)
{
	return vfprintf((FILE*)stream, format, vlist);
}
