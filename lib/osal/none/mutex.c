#include "ub/os/mutex.h"
#include <stddef.h>

UBmutex* ub_mutex_create(UBits args)
{
	(void)args;
	return NULL;
}

void ub_mutex_destroy(UBmutex* mutex)
{
	(void)mutex;
}

void ub_mutex_lock(UBmutex* mutex)
{
	(void)mutex;
}

bool ub_mutex_trylock(UBmutex* mutex)
{
	(void)mutex;
	return true;
}

void ub_mutex_unlock(UBmutex* mutex)
{
	(void)mutex;
}
