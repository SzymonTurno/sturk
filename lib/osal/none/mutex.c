#include "ub/os/mutex.h"
#include "ub/os/mem.h"
#include "ub/logger.h"
#include <stddef.h>

struct UBmutex {
	int locked;
	int recursive;
};

UBmutex* ub_mutex_create(UBits args)
{
	UBmutex* self = ub_malloc(sizeof(*self));

	self->locked = 0;
	self->recursive = args & UB_MUTEX_TYPE_RECURSIVE;
	return self;
}

void ub_mutex_destroy(UBmutex* mutex)
{
	ub_free(mutex);
}

void ub_mutex_lock(UBmutex* mutex)
{
	if (mutex->locked && !mutex->recursive)
		UB_LOG(UB_WARNING, "ub-osal",
		       "Fake mutex does not support context switch.");
	mutex->locked = 1;
}

bool ub_mutex_trylock(UBmutex* mutex)
{
	if (mutex->locked && !mutex->recursive)
		return false;
	mutex->locked = 1;
	return true;
}

void ub_mutex_unlock(UBmutex* mutex)
{
	if (!mutex->locked)
		UB_LOG(UB_WARNING, "ub-osal",
		       "Unlocking an already unlocked mutex.");
	mutex->locked = 0;
}
