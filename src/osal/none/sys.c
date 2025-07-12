#include "cn/os/sys.h"
#include "osal_unistd.h"
#include <stdio.h>
#include <stdlib.h>

int cn_snprintf(char* buffer, size_t bufsz, const char* format, ...)
{
	va_list vlist;
	int ret = 0;

	va_start(vlist, format);
#if __STDC_VERSION__ < 199901L
	(void)bufsz;
	ret = vsprintf(buffer, format, vlist);
#else  /* __STDC_VERSION__ < 199901L */
	ret = vsnprintf(buffer, bufsz, format, vlist);
#endif /* __STDC_VERSION__ < 199901L */
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
