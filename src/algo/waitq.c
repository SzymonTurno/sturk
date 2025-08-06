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

#include "cn/waitq.h"
#include "cantil/cirq.h"
#include "cantil/logger/except.h"
#include "cantil/logger/trace.h"
#include "cantil/os/mem.h"
#include "cantil/os/mutex.h"
#include "cantil/os/sem.h"

struct CnWaitq {
	struct Vertegs* v;
	CnMutex* mut;
	CnSem* sem;
};

CnWaitq* cn_waitq_create(void)
{
	CnWaitq* self = NEW(CnWaitq);

	self->mut = mutex_create(MUTEX_POLICY_PRIO_INHERIT);
	self->sem = sem_create(0);
	self->v = NULL;
	return self;
}

void cn_waitq_destroy(CnWaitq* waitq)
{
	if (!waitq)
		return;

	if (waitq->v)
		TRACE(WARNING, "cantil", "Data loss suspected.");
	sem_destroy(waitq->sem);
	waitq->sem = NULL;
	mutex_destroy(waitq->mut);
	waitq->mut = NULL;
	cn_free(waitq);
}

void cn_waitq_ins(CnWaitq* waitq, struct Vertegs* entry)
{
	ENSURE(waitq, ERROR, null_param);
	mutex_lock(waitq->mut);
	waitq->v = vx_inscirq(waitq->v, entry, -1);
	sem_post(waitq->sem);
	mutex_unlock(waitq->mut);
}

struct Vertegs* cn_waitq_rem(CnWaitq* waitq)
{
	struct Vertegs* entry = NULL;

	ENSURE(waitq, ERROR, null_param);
	sem_wait(waitq->sem);
	mutex_lock(waitq->mut);
	entry = vx_remcirq(&waitq->v, 0);
	mutex_unlock(waitq->mut);
	return entry;
}

struct Vertegs* cn_waitq_tryrem(CnWaitq* waitq)
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
