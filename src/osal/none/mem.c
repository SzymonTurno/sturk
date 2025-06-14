#include "cn/os/mem.h"
#include "cantil/logger/except.h"
#include "cantil/logger/trace.h"
#include <stdlib.h>

void* cn_malloc(size_t size)
{
	void* ret = malloc(size);

	ENSURE(ret, ERROR, alloc_fail);
	return ret;
}

void cn_free(void* ptr)
{
	free(ptr);
}
