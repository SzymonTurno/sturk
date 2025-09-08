/*
BSD 3-Clause License

Copyright (c) 2025, Szymon Turno

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "sturk/os/mutex.h"
#include "sturk/logger/except.h"
#include "sturk/logger/trace.h"
#include "sturk/os/mem.h"
#include <pthread.h>

#define OK   0
#define FAIL 1

struct StMutex {
	pthread_mutex_t pmut;
};

static int setprotocol(pthread_mutexattr_t* attr, StBits args)
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

static int settype(pthread_mutexattr_t* attr, StBits args)
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

StMutex* st_mutex_create(StBits args)
{
	struct StMutex* self = NULL;
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

	self = NEW(struct StMutex);
	if (pthread_mutex_init(&self->pmut, &attr) != OK) {
		/* LCOV_EXCL_START */
		RAISE(ERROR, mutex_fail);
		st_free(self);
		return NULL;
		/* LCOV_EXCL_STOP */
	}

	if (pthread_mutexattr_destroy(&attr) != OK) {
		/* LCOV_EXCL_START */
		RAISE(ERROR, mutex_fail);
		st_free(self);
		return NULL;
		/* LCOV_EXCL_STOP */
	}
	return self;
}

void st_mutex_destroy(StMutex* mutex)
{
	if (pthread_mutex_destroy(&mutex->pmut) != OK)
		RAISE(ERROR, mutex_fail); /* LCOV_EXCL_LINE */
	st_free(mutex);
}

void st_mutex_lock(StMutex* mutex)
{
	if (pthread_mutex_lock(&mutex->pmut) != OK)
		RAISE(ERROR, mutex_fail); /* LCOV_EXCL_LINE */
}

bool st_mutex_trylock(StMutex* mutex)
{
	return pthread_mutex_trylock(&mutex->pmut) == OK;
}

void st_mutex_unlock(StMutex* mutex)
{
	if (pthread_mutex_unlock(&mutex->pmut) != OK)
		RAISE(ERROR, mutex_fail); /* LCOV_EXCL_LINE */
}
