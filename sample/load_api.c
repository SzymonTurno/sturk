#include "pubsub.h"
#include <stdio.h>
#include <stdlib.h>

static size_t size(void)
{
	return sizeof(char**);
}

static void init(CnLoad* load, va_list vlist)
{
	char* buff = malloc(256);

	vsnprintf(buff, 256, va_arg(vlist, char*), vlist);
	*(char**)load = buff;
}

static void deinit(CnLoad* load)
{
	free(*(char**)load);
}

const struct CnLoadVt SAMPLE_LOAD_API[] = {
	{.size = size, .ctor = init, .dtor = deinit}};
