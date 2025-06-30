#include "pubsub.h"
#include <stdio.h>
#include <stdlib.h>

static size_t size(void)
{
	return sizeof(char**);
}

static void init(CnLoad* load, va_list vlist)
{
	const char* format = va_arg(vlist, char*);
	char* buff = malloc(256);

	vsnprintf(buff, 256, format, vlist);
	*(char**)load = buff;
}

static void deinit(CnLoad* load)
{
	free(*(char**)load);
}

const struct CnLoadVt SAMPLE_LOAD_API[] = {
	{.size = size, .ctor = init, .dtor = deinit}};
