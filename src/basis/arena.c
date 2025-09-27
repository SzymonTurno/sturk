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

#include "st/arena.h"
#include "st/arith.h"
#include "sturk/graph.h"
#include "sturk/list.h"

#define THRESHOLD  8
#define CHUNK_SIZE (8 * 1024)

struct Chunk {
	char* avail;
	char* limit;
	struct StArenaGroup* group;
	void* (*alloc_cb)(size_t);
	void (*free_cb)(void*);
};

LIST(struct StArena, struct Chunk);

union Header {
	StArena arena;
	StAlign align;
};

static void insgroup(struct StArenaGroup* group, StArena* arena)
{
	group->p = list_ins(group->p, arena);
	++group->u.n_elems;
}

static StArena* remgroup(struct StArenaGroup* group)
{
	StArena* ret = list_rem(&group->p);

	--group->u.n_elems;
	return ret;
}

static void getchunk(StArena* arena, size_t size, const char* file, int line)
{
	struct StArenaGroup* g = graph_datap(arena)->group;
	StArena* ptr = NULL;
	char* limit = NULL;

	if (g->p) {
		ptr = remgroup(g);
		limit = graph_datap(ptr)->limit;
	} else {
		VX_ASSERT(CHUNK_SIZE > sizeof(union Header) + size);
		ptr = graph_datap(arena)->alloc_cb(CHUNK_SIZE);
		if (!ptr) {
			/* LCOV_EXCL_START */
			st_except(st_except_alloc_fail.reason, file, line);
			return;
			/* LCOV_EXCL_STOP */
		}
		limit = (char*)ptr + CHUNK_SIZE;
	}
	*ptr = *arena;
	graph_datap(arena)->avail = (char*)((union Header*)ptr + 1);
	graph_datap(arena)->limit = limit;
	arena = list_ins(ptr, arena);
}

static size_t roundup(size_t size, size_t align)
{
	/* Note, do not simplify to `size + align - 1`. */
	return ((size + align - 1) / align) * align;
}

static size_t getbytes(StArena* arena)
{
	return (size_t)(graph_datap(arena)->limit - graph_datap(arena)->avail);
}

StArena* st_arena_create(
	struct StArenaGroup* group, void* (*alloc_cb)(size_t),
	void (*free_cb)(void*))
{
	StArena* ret = alloc_cb(sizeof(*ret));

	if (!ret) {
		/* LCOV_EXCL_START */
		st_except(st_except_alloc_fail.reason, __FILE__, __LINE__);
		return NULL;
		/* LCOV_EXCL_STOP */
	}
	ret = list_ins(GRAPH_EMPTY, ret);
	graph_datap(ret)->avail = NULL;
	graph_datap(ret)->limit = NULL;
	graph_datap(ret)->group = group;
	graph_datap(ret)->alloc_cb = alloc_cb;
	graph_datap(ret)->free_cb = free_cb;
	return ret;
}

void st_arena_destroy(StArena* arena)
{
	void (*free_cb)(void*) = NULL;

	if (arena) {
		free_cb = graph_datap(arena)->free_cb;
		st_arena_free(arena);
		free_cb(arena);
	}
}

void* st_arena_alloc(StArena* arena, size_t size, const char* file, int line)
{
	if (!arena)
		return NULL;
	size = roundup(size, sizeof(StAlign));
	while (size > getbytes(arena))
		getchunk(arena, size, file, line);
	graph_datap(arena)->avail += size;
	return graph_datap(arena)->avail - size;
}

void st_arena_free(StArena* arena)
{
	void (*free_cb)(void*) = NULL;
	struct StArenaGroup* g = NULL;
	StArena* p = NULL;
	StArena tmp = {0};

	if (!arena)
		return;

	free_cb = graph_datap(arena)->free_cb;
	while ((p = list_next(arena))) {
		tmp = *p;
		g = graph_datap(p)->group;
		if (g->u.n_elems < THRESHOLD) {
			insgroup(g, p);
			graph_datap(g->p)->limit = graph_datap(arena)->limit;
		} else {
			free_cb(p);
		}
		*arena = tmp;
	}
	VX_ASSERT(!graph_datap(arena)->limit);
	VX_ASSERT(!graph_datap(arena)->avail);
}

void st_arena_cleanup(struct StArenaGroup* group)
{
	StArena* p = NULL;
	void (*free_cb)(void*) = NULL;

	while (group->p) {
		p = remgroup(group);
		free_cb = graph_datap(p)->free_cb;
		free_cb(p);
	}
}
