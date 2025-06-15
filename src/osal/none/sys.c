#include "cn/os/sys.h"
#include "osal_unistd.h"
#include <stdio.h>
#include <stdlib.h>

int cn_snprintf(char* buffer, size_t bufsz, const char* format, ...)
{
	va_list vlist;
	int ret = 0;

	va_start(vlist, format);
	ret = vsnprintf(buffer, bufsz, format, vlist);
	va_end(vlist);
	return ret;
}

/* LCOV_EXCL_START */
void cn_sysfail(void)
{
	exit(EXIT_FAILURE);
}
/* LCOV_EXCL_STOP */

int cn_remove(const char* name)
{
	return remove(name);
}
