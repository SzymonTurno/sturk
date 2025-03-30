#include "ub/os/mutex.h"
#include "ub/os/mem.h"
#include <pthread.h>

#define OK 0

struct UBmutex {
	pthread_mutex_t pmut;
};

static int setprotocol(pthread_mutexattr_t *attr, int protocol)
{
	return pthread_mutexattr_setprotocol(attr, protocol);
}

static int settype(pthread_mutexattr_t *attr, int type)
{
	return pthread_mutexattr_settype(attr, type);
}

static int init(pthread_mutex_t* mutex, pthread_mutexattr_t *attr)
{
	int status = pthread_mutex_init(mutex, attr);

	return status != OK ? status : pthread_mutexattr_destroy(attr);
}

UBmutex* ub_mutex_create(UBits args)
{
	struct UBmutex* self = NULL;
	pthread_mutexattr_t attr;

	if (pthread_mutexattr_init(&attr) != OK)
		return NULL;

	switch (args & UB_MUTEX_POLICY_MASK) {
	case UB_MUTEX_POLICY_PRIO_INHERIT:
		if (setprotocol(&attr, PTHREAD_PRIO_INHERIT) != OK)
			return NULL;
		break;
	default:
		break;
	}

	switch (args & UB_MUTEX_TYPE_MASK) {
	case UB_MUTEX_TYPE_RECURSIVE:
		if (settype(&attr, PTHREAD_MUTEX_RECURSIVE) != OK)
			return NULL;
		break;
	default:
		break;
	}
	self = ub_malloc(sizeof(*self));
	if (init(&self->pmut, &attr) != OK) {
		ub_free(self);
		self = NULL;
	}
	return self;
}

void ub_mutex_destroy(UBmutex* mutex)
{
	pthread_mutex_destroy(&mutex->pmut);
	ub_free(mutex);
}

void ub_mutex_lock(UBmutex* mutex)
{
	pthread_mutex_lock(&mutex->pmut);
}

bool ub_mutex_trylock(UBmutex* mutex)
{
	return pthread_mutex_trylock(&mutex->pmut) == OK;
}

void ub_mutex_unlock(UBmutex* mutex)
{
	pthread_mutex_unlock(&mutex->pmut);
}
