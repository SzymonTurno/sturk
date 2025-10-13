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

#include "sturk/arena.h"
#include "sturk/os/mem.h"
#include "sturk/os/mutex.h"
#include "sturk/os/sys.h"

static struct StArenaGc arena_gc;
static StArena* arena;
static StMutex* mutex;

extern const struct StMemVt STURK_MEM_API[];

void* st_mem_alloc(size_t size, const char* file, int line)
{
	void* ret = NULL;

	if (!arena) {
		arena = arena_create(&arena_gc, STURK_MEM_API);
		mutex = mutex_create(ST_MUTEX_POLICY_PRIO_INHERIT);
	}

	if (mutex) {
		mutex_lock(mutex);
		ret = arena_alloc(arena, size);
		mutex_unlock(mutex);
	} else {
		ret = arena_alloc(arena, size);
	}

	/* LCOV_EXCL_START */
	if (!ret)
		except(st_except_alloc_fail.reason, file, line);
	/* LCOV_EXCL_STOP */
	return ret;
}

void st_mem_free(void* ptr, const char* file, int line)
{
	(void)ptr;
	(void)file;
	(void)line;
}

void st_mem_cleanup(void)
{
	if (mutex)
		mutex_destroy(mutex);
	mutex = NULL;
	if (arena) {
		arena_destroy(arena);
		arena_cleanup(&arena_gc);
	}
	arena = NULL;
}
