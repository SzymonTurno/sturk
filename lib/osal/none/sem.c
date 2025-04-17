#include "ub/os/sem.h"
#include "ub/os/mem.h"
#include "UB/except.h"
#include "UB/logger.h"
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
	ENSURE(sem, ECODES.null_param);
	if (!sem->count)
		LOG(WARNING, "ub-osal",
		       "Fake semaphore does not support context switch.");
	--sem->count;
}

bool ub_sem_trywait(UBsem* sem)
{
	ENSURE(sem, ECODES.null_param);
	if (sem->count) {
		--sem->count;
		return true;
	}
	return false;
}

void ub_sem_post(UBsem* sem)
{
	ENSURE(sem, ECODES.null_param);
	++sem->count;
}
