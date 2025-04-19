#ifndef WAITQ_H
#define WAITQ_H

#include "ub/waitq.h"
#include "UB/cirq.h"
#include "UB/debug/except.h"
#include "UB/debug/log.h"
#include "ub/os/mem.h"
#include "UB/os/mutex.h"
#include "UB/os/sem.h"

struct UBwaitQ {
	UBmutex* mut;
	UBsem* sem;
	struct UBinode* q;
};

UBwaitQ* ub_waitq_create(void)
{
	UBwaitQ* self = ub_malloc(sizeof(*self));

	self->mut = mutex_create(MUTEX_POLICY_PRIO_INHERIT);
	self->sem = sem_create(0);
	self->q = NULL;
	return self;
}

void ub_waitq_destroy(UBwaitQ* waitq)
{
	if (!waitq)
		return;

	if (waitq->q)
		LOG(WARNING, "ub-waitq", "Data loss suspected.");
	sem_destroy(waitq->sem);
	waitq->sem = NULL;
	mutex_destroy(waitq->mut);
	waitq->mut = NULL;
	ub_free(waitq);
}

void ub_waitq_ins(UBwaitQ* waitq, struct UBinode* entry)
{
	ENSURE(waitq, ECODES.null_param);
	mutex_lock(waitq->mut);
	waitq->q = binode_ins(waitq->q, entry, -1);
	sem_post(waitq->sem);
	mutex_unlock(waitq->mut);
}

struct UBinode* ub_waitq_rem(UBwaitQ* waitq)
{
	struct UBinode* entry = NULL;

	ENSURE(waitq, ECODES.null_param);
	sem_wait(waitq->sem);
	mutex_lock(waitq->mut);
	entry = binode_rem(&waitq->q, 0);
	mutex_unlock(waitq->mut);
	return entry;
}

struct UBinode* ub_waitq_tryrem(UBwaitQ* waitq)
{
	struct UBinode* entry = NULL;

	ENSURE(waitq, ECODES.null_param);
	if (sem_trywait(waitq->sem)) {
		mutex_lock(waitq->mut);
		entry = binode_rem(&waitq->q, 0);
		mutex_unlock(waitq->mut);
	}
	return entry;
}

#endif /* WAIT_Q */
