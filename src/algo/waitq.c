#include "cn/waitq.h"
#include "cantil/cirq.h"
#include "cantil/logger/except.h"
#include "cantil/logger/trace.h"
#include "cantil/os/mutex.h"
#include "cantil/os/sem.h"
#include "cn/os/mem.h"

struct CnWaitq {
	CnMutex* mut;
	CnSem* sem;
	struct CnBinode* q;
};

CnWaitq* cn_waitq_create(void)
{
	CnWaitq* self = cn_malloc(sizeof(*self));

	self->mut = mutex_create(MUTEX_POLICY_PRIO_INHERIT);
	self->sem = sem_create(0);
	self->q = NULL;
	return self;
}

void cn_waitq_destroy(CnWaitq* waitq)
{
	if (!waitq)
		return;

	if (waitq->q)
		TRACE(WARNING, "cantil", "Data loss suspected.");
	sem_destroy(waitq->sem);
	waitq->sem = NULL;
	mutex_destroy(waitq->mut);
	waitq->mut = NULL;
	cn_free(waitq);
}

void cn_waitq_ins(CnWaitq* waitq, struct CnBinode* entry)
{
	ENSURE(waitq, ECODES.null_param);
	mutex_lock(waitq->mut);
	waitq->q = binode_ins(waitq->q, entry, -1);
	sem_post(waitq->sem);
	mutex_unlock(waitq->mut);
}

struct CnBinode* cn_waitq_rem(CnWaitq* waitq)
{
	struct CnBinode* entry = NULL;

	ENSURE(waitq, ECODES.null_param);
	sem_wait(waitq->sem);
	mutex_lock(waitq->mut);
	entry = binode_rem(&waitq->q, 0);
	mutex_unlock(waitq->mut);
	return entry;
}

struct CnBinode* cn_waitq_tryrem(CnWaitq* waitq)
{
	struct CnBinode* entry = NULL;

	ENSURE(waitq, ECODES.null_param);
	if (sem_trywait(waitq->sem)) {
		mutex_lock(waitq->mut);
		entry = binode_rem(&waitq->q, 0);
		mutex_unlock(waitq->mut);
	}
	return entry;
}
