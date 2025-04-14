#include "ub/pool.h"
#include "UB/list.h"
#include "UB/arith.h"
#include "UB/logger.h"
#include "ub/os/mem.h"
#include "UB/os/mutex.h"

LIST(union FreeList, void*);

struct UBpool {
	size_t blk_size;
	union FreeList* list;
	UBmutex* mutex;
};

UBpool* ub_pool_create(size_t blk_size)
{
	UBpool* self = ub_malloc(sizeof(*self));

	self->mutex = mutex_create(MUTEX_POLICY_PRIO_INHERIT);
	self->blk_size = max(blk_size, sizeof(*self->list));
	self->list = NULL;
	return self;
}

void ub_pool_destroy(UBpool* pool)
{
	ENSURE(pool, "Null pointer.");
	mutex_lock(pool->mutex);
	while (pool->list)
		ub_free(list_rem(&pool->list));
	mutex_unlock(pool->mutex);
	mutex_destroy(pool->mutex);
	pool->mutex = NULL;
	ub_free(pool);
}

void* ub_pool_alloc(UBpool* pool)
{
	void* ret = NULL;

	ENSURE(pool, "Null pointer.");
	mutex_lock(pool->mutex);
	if (pool->list)
		ret = list_rem(&pool->list);
	mutex_unlock(pool->mutex);
	return ret ? ret : ub_malloc(pool->blk_size);
}

void* ub_pool_tryalloc(UBpool* pool)
{
	void* ret = NULL;

	ENSURE(pool, "Null pointer.");
	mutex_lock(pool->mutex);
	if (pool->list)
		ret = list_rem(&pool->list);
	mutex_unlock(pool->mutex);
	return ret;
}

void ub_pool_free(UBpool* pool, void* blk)
{
	ENSURE(pool, "Null pointer.");
	mutex_lock(pool->mutex);
	pool->list = list_ins(pool->list, (union FreeList*)blk);
	mutex_unlock(pool->mutex);
}
