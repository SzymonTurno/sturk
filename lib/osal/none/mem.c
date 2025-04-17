#include "ub/os/mem.h"
#include "UB/except.h"
#include <stdlib.h>

void* ub_malloc(size_t size)
{
	void* ret = malloc(size);

	ENSURE(ret, ECODES.alloc_fail);
	return ret;
}

void ub_free(void* ptr)
{
	free(ptr);
}
