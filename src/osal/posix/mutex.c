#include "cantil/os/mutex.h"
#include "cantil/logger/except.h"
#include "cantil/logger/trace.h"
#include "cantil/os/mem.h"
#include <pthread.h>

#define OK   0
#define FAIL 1

struct CnMutex {
	pthread_mutex_t pmut;
};

static int setprotocol(pthread_mutexattr_t* attr, CnBits args)
{
	int ret = FAIL;

	switch (args & MUTEX_POLICY_MASK) {
	case MUTEX_POLICY_NONE:
		ret = OK;
		break;
	case MUTEX_POLICY_PRIO_INHERIT:
		ret = pthread_mutexattr_setprotocol(attr, PTHREAD_PRIO_INHERIT);
		ENSURE(ret == OK, ERROR, mutex_fail);
		break;
	default:
		RAISE(WARNING, not_supported);
		break;
	}
	return ret;
}

static int settype(pthread_mutexattr_t* attr, CnBits args)
{
	int ret = FAIL;

	switch (args & MUTEX_TYPE_MASK) {
	case MUTEX_TYPE_NONE:
		ret = OK;
		break;
	case MUTEX_TYPE_RECURSIVE:
		ret = pthread_mutexattr_settype(attr, PTHREAD_MUTEX_RECURSIVE);
		ENSURE(ret == OK, ERROR, mutex_fail);
		break;
	default:
		RAISE(WARNING, not_supported);
		break;
	}
	return ret;
}

CnMutex* cn_mutex_create(CnBits args)
{
	struct CnMutex* self = NULL;
	pthread_mutexattr_t attr;

	if (pthread_mutexattr_init(&attr) != OK) {
		/* LCOV_EXCL_START */
		RAISE(ERROR, mutex_fail);
		return NULL;
		/* LCOV_EXCL_STOP */
	}

	if (setprotocol(&attr, args) != OK) {
		RAISE(WARNING, mutex_fail);
		return NULL;
	}

	if (settype(&attr, args) != OK) {
		RAISE(WARNING, mutex_fail);
		return NULL;
	}

	self = NEW(struct CnMutex);
	if (pthread_mutex_init(&self->pmut, &attr) != OK) {
		/* LCOV_EXCL_START */
		RAISE(ERROR, mutex_fail);
		cn_free(self);
		return NULL;
		/* LCOV_EXCL_STOP */
	}

	if (pthread_mutexattr_destroy(&attr) != OK) {
		/* LCOV_EXCL_START */
		RAISE(ERROR, mutex_fail);
		cn_free(self);
		return NULL;
		/* LCOV_EXCL_STOP */
	}
	return self;
}

void cn_mutex_destroy(CnMutex* mutex)
{
	if (pthread_mutex_destroy(&mutex->pmut) != OK)
		RAISE(ERROR, mutex_fail); /* LCOV_EXCL_LINE */
	cn_free(mutex);
}

void cn_mutex_lock(CnMutex* mutex)
{
	if (pthread_mutex_lock(&mutex->pmut) != OK)
		RAISE(ERROR, mutex_fail); /* LCOV_EXCL_LINE */
}

bool cn_mutex_trylock(CnMutex* mutex)
{
	return pthread_mutex_trylock(&mutex->pmut) == OK;
}

void cn_mutex_unlock(CnMutex* mutex)
{
	if (pthread_mutex_unlock(&mutex->pmut) != OK)
		RAISE(ERROR, mutex_fail); /* LCOV_EXCL_LINE */
}
