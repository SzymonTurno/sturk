#include "cn/os/mem.h"
#include "cantil/logger/except.h"
#include <stdlib.h>

void* cn_malloc(size_t size)
{
	void* ret = malloc(size);

	ENSURE(ret, EXCEPT.ALLOC_FAIL);
	return ret;
}

void cn_free(void* ptr)
{
	free(ptr);
}
