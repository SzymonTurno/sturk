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

#include "st/waitq.h"
#include "sturk/cirq.h"
#include "sturk/io/except.h"
#include "sturk/io/logger.h"
#include "sturk/os/mem.h"
#include "sturk/os/mutex.h"
#include "sturk/os/sem.h"

struct StWaitQ {
	struct Vertegs* v;
	StMutex* mut;
	StSem* sem;
};

StWaitQ* st_waitq_create(void)
{
	StWaitQ* self = NEW(StWaitQ);

	self->mut = mutex_create(MUTEX_POLICY_PRIO_INHERIT);
	self->sem = sem_create(0);
	self->v = NULL;
	return self;
}

void st_waitq_destroy(StWaitQ* waitq)
{
	if (!waitq)
		return;

	if (waitq->v)
		TRACE(WARNING, "sturk", "Data loss suspected.");
	sem_destroy(waitq->sem);
	waitq->sem = NULL;
	mutex_destroy(waitq->mut);
	waitq->mut = NULL;
	st_free(waitq);
}

void st_waitq_ins(StWaitQ* waitq, struct Vertegs* entry)
{
	ENSURE(waitq, ERROR, null_param);
	mutex_lock(waitq->mut);
	waitq->v = vx_inscirq(waitq->v, entry, -1);
	sem_post(waitq->sem);
	mutex_unlock(waitq->mut);
}

struct Vertegs* st_waitq_rem(StWaitQ* waitq)
{
	struct Vertegs* entry = NULL;

	ENSURE(waitq, ERROR, null_param);
	sem_wait(waitq->sem);
	mutex_lock(waitq->mut);
	entry = vx_remcirq(&waitq->v, 0);
	mutex_unlock(waitq->mut);
	return entry;
}

struct Vertegs* st_waitq_tryrem(StWaitQ* waitq)
{
	struct Vertegs* entry = NULL;

	ENSURE(waitq, ERROR, null_param);
	if (sem_trywait(waitq->sem)) {
		mutex_lock(waitq->mut);
		entry = vx_remcirq(&waitq->v, 0);
		mutex_unlock(waitq->mut);
	}
	return entry;
}
