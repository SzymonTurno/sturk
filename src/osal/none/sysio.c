#include "ub/os/sysio.h"
#include <stdio.h>

#ifdef WIN32
#include <io.h>
#include <direct.h>
#define F_OK 0
#define access _access
#define getcwd _getcwd
#else
#include <unistd.h>
#endif

bool ub_fs_fexists(const char* filename)
{
	return access(filename, F_OK) == 0;
}

UBfstream* ub_fs_fopen(const char *filename, const char *mode)
{
	return (UBfstream*)fopen(filename, mode);
}

void ub_fs_fclose(UBfstream* stream)
{
	fclose((FILE*)stream);
}

int ub_fs_fgetc(UBfstream* stream)
{
	return fgetc((FILE*)stream);
}

char* ub_fs_fgets(char* str, int size, UBfstream* stream)
{
	return fgets(str, size, (FILE*)stream);
}

int ub_fs_fseekcur(UBfstream* stream, long int offset)
{
	return fseek((FILE*)stream, offset, SEEK_CUR);
}

UBfstream* ub_fs_stdout(void)
{
	return (UBfstream*)stdout;
}

UBfstream* ub_fs_stderr(void)
{
	return (UBfstream*)stderr;
}

int ub_fs_vprintf(UBfstream* stream, const char* format, va_list vlist)
{
	return vfprintf((FILE*)stream, format, vlist);
}

int ub_snprintf(char* buffer, size_t bufsz, const char* format, ...)
{
	va_list vlist;
	int ret = 0;

	va_start(vlist, format);
	ret = vsnprintf(buffer, bufsz, format, vlist);
	va_end(vlist);
	return ret;
}

int ub_vsnprintf(char* buffer, size_t bufsz, const char* format, va_list vlist)
{
	return vsnprintf(buffer, bufsz, format, vlist);
}

char* ub_getcwd(char* buf, size_t size)
{
	return getcwd(buf, size);
}
