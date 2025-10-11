#include "sturk/arena.h"
#include "sturk/arith.h"
#include "sturk/os/mem.h"
#include "sturk/os/sys.h"

static StAlign heapmem[1024];
static size_t heapp;

static void* getmem(size_t size)
{
	void* ret = NULL;

	if (size) {
		ret = &heapmem[heapp];
		heapp += 1 + (size - 1) / sizeof(heapmem[0]);
	}
	return ret;
}

static void freemem(void* ptr)
{
	(void)ptr;
}

const struct StMemVt STURK_MEM_API[] = {
	{.alloc_cb = getmem, .free_cb = freemem}};

int _start()
{
	char* s = NEW(char, 128);

	strprint(s, "%s", "test");
	st_free(s);
	return 0;
}
