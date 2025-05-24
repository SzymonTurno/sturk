#include "UB/os/mutex.h"
#include "ub/os/mem.h"
#include "UB/logger/except.h"
#include <pthread.h>

#define OK 0
#define FAIL 1

struct UBmutex {
	pthread_mutex_t pmut;
};

static int setprotocol(pthread_mutexattr_t *attr, UBits args)
{
	switch (args & MUTEX_POLICY_MASK) {
	case MUTEX_POLICY_NONE:
		return OK;
	case MUTEX_POLICY_PRIO_INHERIT:
		return pthread_mutexattr_setprotocol(attr,
			PTHREAD_PRIO_INHERIT);
	default:
		RAISE(ECODES.not_supported);
		break;
	}
	return FAIL;
}

static int settype(pthread_mutexattr_t *attr, UBits args)
{
	switch (args & MUTEX_TYPE_MASK) {
	case MUTEX_TYPE_NONE:
		return OK;
	case MUTEX_TYPE_RECURSIVE:
		return pthread_mutexattr_settype(attr, PTHREAD_MUTEX_RECURSIVE);
	default:
		RAISE(ECODES.not_supported);
		break;
	}
	return FAIL;
}

UBmutex* ub_mutex_create(UBits args)
{
	struct UBmutex* self = NULL;
	pthread_mutexattr_t attr;

	if (pthread_mutexattr_init(&attr) != OK) {
		RAISE(ECODES.mutex_fail);
		return NULL;
	}

	if (setprotocol(&attr, args) != OK) {
		RAISE(ECODES.mutex_fail);
		return NULL;
	}

	if (settype(&attr, args) != OK) {
		RAISE(ECODES.mutex_fail);
		return NULL;
	}
	self = ub_malloc(sizeof(*self));
	if (pthread_mutex_init(&self->pmut, &attr) != OK) {
		RAISE(ECODES.mutex_fail);
		ub_free(self);
		return NULL;
	}

	if (pthread_mutexattr_destroy(&attr) != OK) {
		RAISE(ECODES.mutex_fail);
		ub_free(self);
		return NULL;
	}
	return self;
}

void ub_mutex_destroy(UBmutex* mutex)
{
	if (pthread_mutex_destroy(&mutex->pmut) != OK)
		RAISE(ECODES.mutex_fail);
	ub_free(mutex);
}

void ub_mutex_lock(UBmutex* mutex)
{
	if (pthread_mutex_lock(&mutex->pmut) != OK)
		RAISE(ECODES.mutex_fail);
}

bool ub_mutex_trylock(UBmutex* mutex)
{
	return pthread_mutex_trylock(&mutex->pmut) == OK;
}

void ub_mutex_unlock(UBmutex* mutex)
{
	if (pthread_mutex_unlock(&mutex->pmut) != OK)
		RAISE(ECODES.mutex_fail);
}
