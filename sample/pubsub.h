#ifndef PUBSUB_H
#define PUBSUB_H

#include "cn/str.h"
#include <stdio.h>
#include <stdlib.h>

static size_t sample_loadsize(void)
{
	return sizeof(char**);
}

static void sample_loadinit(CnLoad* load, va_list vlist)
{
	char* buff = malloc(256);

	vsnprintf(buff, 256, va_arg(vlist, char*), vlist);
	*(char**)load = buff;
}

static void sample_loaddeinit(CnLoad* load)
{
	free(*(char**)load);
}

static const struct CnLoadVt SAMPLE_LOAD_API[] = {
	{.size = sample_loadsize, .ctor = sample_loadinit, .dtor = sample_loaddeinit}};

struct CnStrq* single_thread_pubsub(void);

struct CnStrbag* multi_thread_pubsub(void);

#endif /* PUBSUB_H */
