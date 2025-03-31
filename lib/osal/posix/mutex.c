#include "ub/os/mutex.h"
#include "ub/debug.h"
#include <pthread.h>

#define OK 0

struct UBmutex {
	pthread_mutex_t pmut;
};

static void setprotocol(pthread_mutexattr_t *attr, int protocol)
{
	if (pthread_mutexattr_setprotocol(attr, protocol) != OK)
		ub_raise("mutex failure");
}

static void settype(pthread_mutexattr_t *attr, int type)
{
	if (pthread_mutexattr_settype(attr, type) != OK)
		ub_raise("mutex failure");
}

static void init(pthread_mutex_t* mutex, pthread_mutexattr_t *attr)
{
	if (pthread_mutex_init(mutex, attr) != OK)
		ub_raise("mutex failure");

	if (pthread_mutexattr_destroy(attr) != OK)
		ub_raise("mutex failure");
}

UBmutex* ub_mutex_create(UBits args)
{
	struct UBmutex* self = NULL;
	pthread_mutexattr_t attr;

	if (pthread_mutexattr_init(&attr) != OK)
		return NULL;

	switch (args & UB_MUTEX_POLICY_MASK) {
	case UB_MUTEX_POLICY_PRIO_INHERIT:
		setprotocol(&attr, PTHREAD_PRIO_INHERIT);
		break;
	default:
		break;
	}

	switch (args & UB_MUTEX_TYPE_MASK) {
	case UB_MUTEX_TYPE_RECURSIVE:
		settype(&attr, PTHREAD_MUTEX_RECURSIVE);
		break;
	default:
		break;
	}
	self = ub_malloc(sizeof(*self));
	init(&self->pmut, &attr);
	return self;
}

void ub_mutex_destroy(UBmutex* mutex)
{
	if (pthread_mutex_destroy(&mutex->pmut) != OK)
		ub_raise("mutex failure");
	ub_free(mutex);
}

void ub_mutex_lock(UBmutex* mutex)
{
	if (pthread_mutex_lock(&mutex->pmut) != OK)
		ub_raise("mutex failure");
}

bool ub_mutex_trylock(UBmutex* mutex)
{
	return pthread_mutex_trylock(&mutex->pmut) == OK;
}

void ub_mutex_unlock(UBmutex* mutex)
{
	if (pthread_mutex_unlock(&mutex->pmut) != OK)
		ub_raise("mutex failure");
}
