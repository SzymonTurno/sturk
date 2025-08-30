/*
BSD 3-Clause License

Copyright (c) 2025, Szymon Turno

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "st/arith.h"
#include "st/os/mem.h"
#include <stdlib.h>

#define PAGE_SIZE (8 * 1024)

struct Chunk {
	char* avail;
	char* limit;
};

LIST(struct Arena, struct Chunk);

union Header {
	struct Arena b;
	StAlign a;
};

static struct Arena freechunks;
static int n_free;

static struct Arena* arena_create(void)
{
	struct Arena* ret = list_ins(NULL, malloc(sizeof(*ret)));

	if (!ret)
		st_except(/* LCOV_EXCL_LINE */
		          "Memory allocation failed.", __FILE__, __LINE__);
	list_datap(ret)->avail = NULL;
	list_datap(ret)->limit = NULL;
	return ret;
}

static void arena_dispose(struct Arena* arena)
{
	if (!arena)
		st_except("Null param.", __FILE__, __LINE__);
	arena_free(arena);
	free(arena);
}

static void getchunk(struct Arena* arena, size_t size)
{
	struct Arena* ptr = NULL;
	char* limit = NULL;
	size_t m = 0;

	if ((ptr = freechunks) != NULL) {
		freechunks =
			graph_4vx(graph_2vx(freechunks)->nbor[0], freechunks);
		--n_free;
		limit = ptr->limit;
	} else {
		m = sizeof(union Header) + size + PAGE_SIZE;
		ptr = malloc(m);
		if (!ptr)
			st_except(/* LCOV_EXCL_LINE */
			          "Memory allocation failed.", __FILE__,
			          __LINE__);
		limit = (char*)ptr + m;
	}
	*ptr = *arena;
	arena->avail = (char*)((union Header*)ptr + 1);
	arena->limit = limit;
	arena->prev = ptr;
}

static void*
arena_alloc(struct Arena* arena, size_t size, const char* file, int line)
{
	if (!arena)
		st_except("Null param.", __FILE__, __LINE__);

	size = ((size + sizeof(StAlign) - 1) / sizeof(StAlign)) *
	       sizeof(StAlign);
	while (size > arena->limit - arena->avail)
		getchunk(arena, size);
	arena->avail += size;
	return arena->avail - size;
}

void* st_mem_alloc(size_t size, const char* file, int line)
{
	(void)size;
	(void)file;
	(void)line;
	return NULL;
}

void st_mem_free(void* ptr, const char* file, int line)
{
	(void)ptr;
	(void)file;
	(void)line;
}
