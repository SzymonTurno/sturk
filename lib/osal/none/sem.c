#include "ub/os/sem.h"
#include "ub/os/mem.h"
#include "ub/logger.h"
#include <stddef.h>

struct UBsem {
	int count;
};

UBsem* ub_sem_create(UBits args)
{
	UBsem* self = ub_malloc(sizeof(*self));

	(void)args;
	self->count = 0;
	return self;
}

void ub_sem_destroy(UBsem* sem)
{
	ub_free(sem);
}

void ub_sem_wait(UBsem* sem)
{
	UB_ENSURE(sem, "Null pointer.");
	if (!sem->count)
		UB_LOG(UB_WARNING, "ub-osal",
		       "Fake semaphore does not support context switch.");
	--sem->count;
}

bool ub_sem_trywait(UBsem* sem)
{
	UB_ENSURE(sem, "Null pointer.");
	if (sem->count) {
		--sem->count;
		return true;
	}
	return false;
}

void ub_sem_post(UBsem* sem)
{
	UB_ENSURE(sem, "Null pointer.");
	++sem->count;
}
