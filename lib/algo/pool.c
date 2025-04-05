#include "ub/pool.h"
#include "ub/list.h"
#include "ub/arith.h"
#include "ub/debug.h"
#include "ub/os/mutex.h"

UB_LIST(union FreeList, void*);

struct UBpool {
	size_t blk_size;
	union FreeList* list;
	UBmutex* mutex;
};

UBpool* ub_pool_create(size_t blk_size)
{
	UBpool* self = ub_malloc(sizeof(*self));

	self->mutex = ub_mutex_create(UB_MUTEX_POLICY_PRIO_INHERIT);
	self->blk_size = ub_max(blk_size, sizeof(*self->list));
	self->list = NULL;
	return self;
}

void ub_pool_destroy(UBpool* pool)
{
	ub_ensure(pool, "Null pointer.");
	ub_mutex_lock(pool->mutex);
	while (pool->list)
		ub_free(ub_list_rem(&pool->list));
	ub_mutex_unlock(pool->mutex);
	ub_mutex_destroy(pool->mutex);
	pool->mutex = NULL;
	ub_free(pool);
}

void* ub_pool_alloc(UBpool* pool)
{
	void* ret = NULL;

	ub_ensure(pool, "Null pointer.");
	ub_mutex_lock(pool->mutex);
	if (pool->list)
		ret = ub_list_rem(&pool->list);
	ub_mutex_unlock(pool->mutex);
	return ret ? ret : ub_malloc(pool->blk_size);
}

void* ub_pool_tryalloc(UBpool* pool)
{
	void* ret = NULL;

	ub_ensure(pool, "Null pointer.");
	ub_mutex_lock(pool->mutex);
	if (pool->list)
		ret = ub_list_rem(&pool->list);
	ub_mutex_unlock(pool->mutex);
	return ret;
}

void ub_pool_free(UBpool* pool, void* blk)
{
	ub_ensure(pool, "Null pointer.");
	ub_mutex_lock(pool->mutex);
	pool->list = ub_list_ins(pool->list, (union FreeList*)blk);
	ub_mutex_unlock(pool->mutex);
}
