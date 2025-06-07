#include "cantil/os/mutex.h"
#include "cantil/logger/except.h"
#include "cantil/logger/trace.h"
#include "cn/os/mem.h"
#include <stddef.h>

struct CnMutex {
	int locked;
	int recursive;
};

CnMutex* cn_mutex_create(CnBits args)
{
	CnMutex* self = cn_malloc(sizeof(*self));

	self->locked = 0;
	self->recursive = args & MUTEX_TYPE_RECURSIVE;
	return self;
}

void cn_mutex_destroy(CnMutex* mutex) { cn_free(mutex); }

void cn_mutex_lock(CnMutex* mutex)
{
	ENSURE(mutex, ECODES.null_param);
	if (mutex->locked && !mutex->recursive)
		TRACE(WARNING, "cantil",
		      "Fake mutex does not support context switch.");
	mutex->locked = 1;
}

bool cn_mutex_trylock(CnMutex* mutex)
{
	ENSURE(mutex, ECODES.null_param);
	if (mutex->locked && !mutex->recursive)
		return false;
	mutex->locked = 1;
	return true;
}

void cn_mutex_unlock(CnMutex* mutex)
{
	ENSURE(mutex, ECODES.null_param);
	if (!mutex->locked)
		TRACE(WARNING, "cantil",
		      "Unlocking an already unlocked mutex.");
	mutex->locked = 0;
}
