#include "cn/pool.h"
#include "cantil/arith.h"
#include "cantil/list.h"
#include "cantil/logger/except.h"
#include "cantil/os/mutex.h"
#include "cn/os/mem.h"

LIST(union FreeList, void*);

struct CnPool {
	size_t blk_size;
	union FreeList* list;
	CnMutex* mutex;
};

CnPool* cn_pool_create(size_t blk_size)
{
	CnPool* self = cn_malloc(sizeof(*self));

	self->mutex = mutex_create(MUTEX_POLICY_PRIO_INHERIT);
	self->blk_size = max(blk_size, sizeof(*self->list));
	self->list = NULL;
	return self;
}

void cn_pool_destroy(CnPool* pool)
{
	if (!pool)
		return;
	mutex_lock(pool->mutex);
	while (pool->list)
		cn_free(list_rem(&pool->list));
	mutex_unlock(pool->mutex);
	mutex_destroy(pool->mutex);
	pool->mutex = NULL;
	cn_free(pool);
}

void* cn_pool_alloc(CnPool* pool)
{
	void* ret = NULL;

	ENSURE(pool, ECODES.null_param);
	mutex_lock(pool->mutex);
	if (pool->list)
		ret = list_rem(&pool->list);
	mutex_unlock(pool->mutex);
	return ret ? ret : cn_malloc(pool->blk_size);
}

void* cn_pool_tryalloc(CnPool* pool)
{
	void* ret = NULL;

	ENSURE(pool, ECODES.null_param);
	mutex_lock(pool->mutex);
	if (pool->list)
		ret = list_rem(&pool->list);
	mutex_unlock(pool->mutex);
	return ret;
}

void cn_pool_free(CnPool* pool, void* blk)
{
	ENSURE(pool, ECODES.null_param);
	mutex_lock(pool->mutex);
	pool->list = list_ins(pool->list, (union FreeList*)blk);
	mutex_unlock(pool->mutex);
}
