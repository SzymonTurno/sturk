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
	struct StArenaGc* gc;
	const struct StMemVt* vp;
	void* padding;
};

LIST(struct StArena, struct Chunk);

union Header {
	StArena arena;
	StAlign align;
};

static void insgc(struct StArenaGc* gc, StArena* arena)
{
	gc->p = list_ins(gc->p, arena);
	++gc->u.n_elems;
}

static StArena* remgc(struct StArenaGc* gc)
{
	StArena* ret = list_rem(&gc->p);

	--gc->u.n_elems;
	return ret;
}

static void getchunk(StArena* arena, size_t size)
{
	struct StArenaGc* gc = graph_datap(arena)->gc;
	StArena* ptr = NULL;
	char* limit = NULL;

	if (gc->p) {
		ptr = remgc(gc);
		limit = graph_datap(ptr)->limit;
	} else {
		VX_ASSERT(CHUNK_SIZE > sizeof(union Header) + size);
		ptr = graph_datap(arena)->vp->alloc_cb(CHUNK_SIZE);
		VX_ASSERT(ptr);
		/* LCOV_EXCL_START */
		if (!ptr)
			return;
		/* LCOV_EXCL_STOP */
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

StArena* st_arena_create(struct StArenaGc* gc, const struct StMemVt* vp)
{
	StArena* ret = vp->alloc_cb(sizeof(*ret));

	VX_ENSURE_MEM(ret);
	ret = list_ins(GRAPH_EMPTY, ret);
	graph_datap(ret)->avail = NULL;
	graph_datap(ret)->limit = NULL;
	graph_datap(ret)->gc = gc;
	graph_datap(ret)->vp = vp;
	return ret;
}

void st_arena_destroy(StArena* arena)
{
	void (*free_cb)(void*) = NULL;

	if (arena) {
		free_cb = graph_datap(arena)->vp->free_cb;
		st_arena_free(arena);
		free_cb(arena);
	}
}

void* st_arena_alloc(StArena* arena, size_t size)
{
	if (!arena)
		return NULL;
	size = roundup(size, sizeof(StAlign));
	while (size > getbytes(arena))
		getchunk(arena, size);
	graph_datap(arena)->avail += size;
	return graph_datap(arena)->avail - size;
}

void st_arena_free(StArena* arena)
{
	void (*free_cb)(void*) = NULL;
	struct StArenaGc* gc = NULL;
	StArena* p = NULL;
	StArena tmp = {0};

	if (!arena)
		return;

	free_cb = graph_datap(arena)->vp->free_cb;
	while ((p = list_next(arena))) {
		tmp = *p;
		gc = graph_datap(p)->gc;
		if (gc->u.n_elems < THRESHOLD) {
			insgc(gc, p);
			graph_datap(gc->p)->limit = graph_datap(arena)->limit;
		} else {
			free_cb(p);
		}
		*arena = tmp;
	}
	VX_ASSERT(!graph_datap(arena)->limit);
	VX_ASSERT(!graph_datap(arena)->avail);
}

void st_arena_cleanup(struct StArenaGc* gc)
{
	StArena* p = NULL;
	void (*free_cb)(void*) = NULL;

	while (gc->p) {
		p = remgc(gc);
		free_cb = graph_datap(p)->vp->free_cb;
		free_cb(p);
	}
}
