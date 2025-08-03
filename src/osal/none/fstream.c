#include "cn/os/fstream.h"
#include "osal_unistd.h"
#include <stdio.h>

CnFstream* cn_fopen(const char* filename, const char* mode)
{
	return (CnFstream*)fopen(filename, mode);
}

void cn_fclose(CnFstream* stream)
{
	fclose((FILE*)stream);
}

char* cn_fgets(char* str, int size, CnFstream* stream)
{
	return fgets(str, size, (FILE*)stream);
}

int cn_fseekset(CnFstream* stream, long int offset)
{
	return fseek((FILE*)stream, offset, SEEK_SET);
}

CnFstream* cn_stdout(void)
{
	return (CnFstream*)stdout;
}

CnFstream* cn_stderr(void)
{
	return (CnFstream*)stderr;
}

int cn_vfprintf(CnFstream* stream, const char* format, va_list vlist)
{
	return vfprintf((FILE*)stream, format, vlist);
}
