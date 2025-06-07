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

int cn_vsnprintf(char* buffer, size_t bufsz, const char* format, va_list vlist)
{
	return vsnprintf(buffer, bufsz, format, vlist);
}

char* cn_getcwd(char* buf, size_t size) { return getcwd(buf, size); }

void cn_sysfail(void) { exit(EXIT_FAILURE); }

bool cn_fexists(const char* filename) { return access(filename, F_OK) == 0; }

int cn_remove(const char* name) { return remove(name); }
