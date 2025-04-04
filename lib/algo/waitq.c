#ifndef WAITQ_H
#define WAITQ_H

#include "ub/waitq.h"
#include "ub/os/mem.h"
#include "ub/os/mutex.h"
#include "ub/os/sem.h"

struct UBwaitQ {
	UBmutex* mut;
	UBsem* sem;
	struct UBinode* q;
};

UBwaitQ* ub_waitq_create(void)
{
	UBwaitQ* self = ub_malloc(sizeof(*self));

	self->mut = ub_mutex_create(UB_MUTEX_POLICY_PRIO_INHERIT);
	self->sem = ub_sem_create(0);
	self->q = NULL;
	return self;
}

void ub_waitq_destroy(UBwaitQ* waitq)
{
	ub_ensure(waitq, "Bad pointer.");
	ub_sem_destroy(waitq->sem);
	waitq->sem = NULL;
	ub_mutex_destroy(waitq->mut);
	waitq->mut = NULL;
	ub_free(waitq);
}

void ub_waitq_ins(UBwaitQ* waitq, struct UBinode* entry)
{
	ub_ensure(waitq, "Bad pointer.");
	ub_mutex_lock(waitq->mut);
	waitq->q = ub_binode_ins(waitq->q, entry);
	ub_sem_post(waitq->sem);
	ub_mutex_unlock(waitq->mut);
}

struct UBinode* ub_waitq_rem(UBwaitQ* waitq)
{
	struct UBinode* entry = NULL;

	ub_ensure(waitq, "Bad pointer.");
	ub_sem_wait(waitq->sem);
	ub_mutex_lock(waitq->mut);
	entry = ub_binode_rem(&waitq->q);
	ub_mutex_unlock(waitq->mut);
	return entry;
}

#endif /* WAIT_Q */
