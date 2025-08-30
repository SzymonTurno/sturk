/*
 * File included as an example in the documentation.
 *
 * Note, line numbering in this file must be coherent with the documentation.
 */

#include "pubsub.h"
#include <stdio.h>
#include <stdlib.h>

static size_t size(void)
{
	return sizeof(char[8]);
}

static void init(StLoad* load, va_list vlist)
{
	const char* format = va_arg(vlist, char*);

	vsnprintf(load, 8, format, vlist);
}

static void deinit(StLoad* load)
{
	(void)load;
}

const struct StLoadVt SAMPLE_LOAD_API[] = {
	{.size = size, .ctor = init, .dtor = deinit}};
