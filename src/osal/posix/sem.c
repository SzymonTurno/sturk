#include "cn/os/sem.h"
#include "cantil/logger/except.h"
#include "cantil/logger/trace.h"
#include "cantil/os/mem.h"
#include <semaphore.h>

#define OK 0

struct CnSem {
	sem_t sem;
};

CnSem* cn_sem_create(CnBits args)
{
	struct CnSem* self = NEW(struct CnSem);
	(void)args;

	if (sem_init(&self->sem, 0, 0) != OK) {
		/* LCOV_EXCL_START */
		RAISE(ERROR, sem_fail);
		cn_free(self);
		return NULL;
		/* LCOV_EXCL_STOP */
	}
	return self;
}

void cn_sem_destroy(CnSem* sem)
{
	if (sem_destroy(&sem->sem) != OK)
		RAISE(ERROR, sem_fail); /* LCOV_EXCL_LINE */
	cn_free(sem);
}

void cn_sem_wait(CnSem* sem)
{
	if (sem_wait(&sem->sem) != OK)
		RAISE(ERROR, sem_fail); /* LCOV_EXCL_LINE */
}

bool cn_sem_trywait(CnSem* sem)
{
	return sem_trywait(&sem->sem) == OK;
}

void cn_sem_post(CnSem* sem)
{
	if (sem_post(&sem->sem) != OK)
		RAISE(ERROR, sem_fail); /* LCOV_EXCL_LINE */
}
