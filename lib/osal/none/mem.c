#include "ub/os/mem.h"
#include "ub/debug.h"
#include <stdlib.h>

void* ub_malloc(size_t size)
{
	void* ret = malloc(size);

	ub_ensure(ret, "Memory allocation failed.");
	return ret;
}

void ub_free(void* ptr)
{
	free(ptr);
}
