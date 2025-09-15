#include "pubsub.h"
#include <stdio.h>
#include <stdlib.h>

static size_t size(void)
{
	return sizeof(char*);
}

static void init(void* msg, va_list vlist)
{
	int n = va_arg(vlist, int) + 1;
	char* str = NULL;

	if (n > 0) {
		str = malloc((size_t)n);
		str[--n] = '\0';
		while (n--)
			str[n] = '|';
	}
	*(char**)msg = str;
}

static void deinit(void* msg)
{
	free(*(char**)msg);
}

const struct StMessageVt SAMPLE_MESSAGE_API[] = {
	{.size = size, .ctor = init, .dtor = deinit}};
