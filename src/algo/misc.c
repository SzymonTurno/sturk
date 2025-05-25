#include "ub/misc.h"
#include "ub/os/mem.h"
#include <string.h>

char* ub_newstr(const char* str)
{
	return strcpy(ub_malloc(strlen(str) + 1), str);
}
