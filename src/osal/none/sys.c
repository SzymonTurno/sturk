#include "ub/os/sys.h"
#include "osal_unistd.h"
#include <stdio.h>
#include <stdlib.h>

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

void ub_sysfail(void)
{
	exit(EXIT_FAILURE);
}

int ub_remove(const char* name)
{
	return remove(name);
}
