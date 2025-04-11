#include "ub/os/mem.h"
#include "ub/logger.h"
#include <stdlib.h>

void* ub_malloc(size_t size)
{
	void* ret = malloc(size);

	UB_ENSURE(ret, "Memory allocation failed.");
	return ret;
}

void ub_free(void* ptr)
{
	free(ptr);
}
