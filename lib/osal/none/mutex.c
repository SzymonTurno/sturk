#include "UB/os/mutex.h"
#include "ub/os/mem.h"
#include "UB/debug/except.h"
#include "UB/debug/log.h"
#include <stddef.h>

struct UBmutex {
	int locked;
	int recursive;
};

UBmutex* ub_mutex_create(UBits args)
{
	UBmutex* self = ub_malloc(sizeof(*self));

	self->locked = 0;
	self->recursive = args & MUTEX_TYPE_RECURSIVE;
	return self;
}

void ub_mutex_destroy(UBmutex* mutex)
{
	ub_free(mutex);
}

void ub_mutex_lock(UBmutex* mutex)
{
	ENSURE(mutex, ECODES.null_param);
	if (mutex->locked && !mutex->recursive)
		LOG(WARNING, "ub-osal",
		       "Fake mutex does not support context switch.");
	mutex->locked = 1;
}

bool ub_mutex_trylock(UBmutex* mutex)
{
	ENSURE(mutex, ECODES.null_param);
	if (mutex->locked && !mutex->recursive)
		return false;
	mutex->locked = 1;
	return true;
}

void ub_mutex_unlock(UBmutex* mutex)
{
	ENSURE(mutex, ECODES.null_param);
	if (!mutex->locked)
		LOG(WARNING, "ub-osal", "Unlocking an already unlocked mutex.");
	mutex->locked = 0;
}
