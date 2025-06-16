#ifndef PUBSUB_H
#define PUBSUB_H

#include "cn/str.h"
#include <stdio.h>
#include <stdlib.h>

static size_t default_size(void)
{
	return sizeof(char**);
}

static void default_init(CnLoad* load, va_list vlist)
{
	char* buff = malloc(256);

	vsnprintf(buff, 256, va_arg(vlist, char*), vlist);
	*(char**)load = buff;
}

static void default_deinit(CnLoad* load)
{
	free(*(char**)load);
}

static const struct CnLoadVt DEFAULT_LOAD_VP[] = {
	{.size = default_size, .ctor = default_init, .dtor = default_deinit}};

struct CnStrq* single_thread_pubsub(void);

struct CnStrbag* multi_thread_pubsub(void);

#endif /* PUBSUB_H */
