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

#include "st/pool.h"
#include "sturk/arith.h"
#include "sturk/list.h"
#include "sturk/logger/except.h"
#include "sturk/logger/trace.h"
#include "sturk/os/mem.h"
#include "sturk/os/mutex.h"

LIST(union FreeList, void*);

struct StPool {
	size_t blk_size;
	union FreeList* list;
	StMutex* mutex;
};

StPool* st_pool_create(size_t blk_size)
{
	StPool* self = NEW(StPool);

	self->mutex = mutex_create(MUTEX_POLICY_PRIO_INHERIT);
	self->blk_size = MAX(blk_size, sizeof(*self->list));
	self->list = NULL;
	return self;
}

void st_pool_destroy(StPool* pool)
{
	if (!pool)
		return;
	mutex_lock(pool->mutex);
	while (pool->list)
		st_free(list_rem(&pool->list));
	mutex_unlock(pool->mutex);
	mutex_destroy(pool->mutex);
	pool->mutex = NULL;
	st_free(pool);
}

void* st_pool_alloc(StPool* pool)
{
	void* ret = NULL;

	ENSURE(pool, ERROR, null_param);
	mutex_lock(pool->mutex);
	if (pool->list)
		ret = list_rem(&pool->list);
	mutex_unlock(pool->mutex);
	return ret ? ret : st_malloc(pool->blk_size);
}

void* st_pool_tryalloc(StPool* pool)
{
	void* ret = NULL;

	ENSURE(pool, ERROR, null_param);
	mutex_lock(pool->mutex);
	if (pool->list)
		ret = list_rem(&pool->list);
	mutex_unlock(pool->mutex);
	return ret;
}

void st_pool_free(StPool* pool, void* blk)
{
	ENSURE(pool, ERROR, null_param);
	mutex_lock(pool->mutex);
	pool->list = list_ins(pool->list, (union FreeList*)blk);
	mutex_unlock(pool->mutex);
}
