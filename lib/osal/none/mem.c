#include "ub/os/mem.h"
#include <stdlib.h>

void* ub_malloc(size_t size)
{
	return malloc(size);
}

void ub_free(void* ptr)
{
	free(ptr);
}
