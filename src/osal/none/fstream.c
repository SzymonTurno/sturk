#include "ub/os/fstream.h"
#include "osal_unistd.h"
#include <stdio.h>

bool ub_fexists(const char* filename)
{
	return access(filename, F_OK) == 0;
}

UBfstream* ub_fopen(const char* filename, const char* mode)
{
	return (UBfstream*)fopen(filename, mode);
}

void ub_fclose(UBfstream* stream)
{
	fclose((FILE*)stream);
}

int ub_fgetc(UBfstream* stream)
{
	return fgetc((FILE*)stream);
}

char* ub_fgets(char* str, int size, UBfstream* stream)
{
	return fgets(str, size, (FILE*)stream);
}

int ub_fputs(const char* str, UBfstream* stream)
{
	return fputs(str, (FILE*)stream);
}

int ub_fseekset(UBfstream* stream, long int offset)
{
	return fseek((FILE*)stream, offset, SEEK_SET);
}

int ub_fseekcur(UBfstream* stream, long int offset)
{
	return fseek((FILE*)stream, offset, SEEK_CUR);
}

int ub_fseekend(UBfstream* stream, long int offset)
{
	return fseek((FILE*)stream, offset, SEEK_END);
}

UBfstream* ub_stdout(void)
{
	return (UBfstream*)stdout;
}

UBfstream* ub_stderr(void)
{
	return (UBfstream*)stderr;
}

int ub_fvprintf(UBfstream* stream, const char* format, va_list vlist)
{
	return vfprintf((FILE*)stream, format, vlist);
}
