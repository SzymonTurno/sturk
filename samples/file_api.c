#include "pubsub.h"
#include <stdio.h>

static void file_putc(void* p, char c)
{
	FILE* fp = p;

	putc(c, fp);
}

static char file_getc(void* p)
{
	FILE* fp = p;

	return (char)getc(fp);
}

const struct StIoVt SAMPLE_FILE_API[] = {
	{.putc_cb = file_putc, .getc_cb = file_getc}};
