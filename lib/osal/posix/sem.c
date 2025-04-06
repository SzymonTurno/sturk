#include "ub/os/sem.h"
#include "ub/debug.h"
#include <semaphore.h>

#define OK 0

struct UBsem {
	sem_t sem;
};

UBsem* ub_sem_create(UBits args)
{
	struct UBsem* self = ub_malloc(sizeof(*self));
	(void) args;

	if (sem_init(&self->sem, 0, 0) != OK)
		ub_raise("Semaphore failure.");
	return self;
}

void ub_sem_destroy(UBsem* sem)
{
	if (sem_destroy(&sem->sem) != OK)
		ub_raise("Semaphore failure.");
	ub_free(sem);
}

void ub_sem_wait(UBsem* sem)
{
	if (sem_wait(&sem->sem) != OK)
		ub_raise("Semaphore failure.");
}

bool ub_sem_trywait(UBsem* sem)
{
	return sem_trywait(&sem->sem) == OK;
}

void ub_sem_post(UBsem* sem)
{
	if (sem_post(&sem->sem) != OK)
		ub_raise("Semaphore failure.");
}
