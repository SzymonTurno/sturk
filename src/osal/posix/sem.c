#include "cn/os/sem.h"
#include "cantil/logger/except.h"
#include "cn/os/mem.h"
#include <semaphore.h>

#define OK 0

struct CnSem {
	sem_t sem;
};

CnSem* cn_sem_create(CnBits args)
{
	struct CnSem* self = cn_malloc(sizeof(*self));
	(void)args;

	if (sem_init(&self->sem, 0, 0) != OK) {
		RAISE(ECODES.sem_fail);
		cn_free(self);
		return NULL;
	}
	return self;
}

void cn_sem_destroy(CnSem* sem)
{
	if (sem_destroy(&sem->sem) != OK)
		RAISE(ECODES.sem_fail);
	cn_free(sem);
}

void cn_sem_wait(CnSem* sem)
{
	if (sem_wait(&sem->sem) != OK)
		RAISE(ECODES.sem_fail);
}

bool cn_sem_trywait(CnSem* sem) { return sem_trywait(&sem->sem) == OK; }

void cn_sem_post(CnSem* sem)
{
	if (sem_post(&sem->sem) != OK)
		RAISE(ECODES.sem_fail);
}
