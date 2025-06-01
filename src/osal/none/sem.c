#include "cn/os/sem.h"
#include "cn/os/mem.h"
#include "cantil/logger/except.h"
#include "cantil/logger/log.h"
#include <stddef.h>

struct CnSem {
	int n;
};

CnSem* cn_sem_create(CnBits args)
{
	CnSem* self = cn_malloc(sizeof(*self));

	(void)args;
	self->n = 0;
	return self;
}

void cn_sem_destroy(CnSem* sem)
{
	cn_free(sem);
}

void cn_sem_wait(CnSem* sem)
{
	ENSURE(sem, ECODES.null_param);
	if (!sem->n)
		LOG(WARNING, "cantil",
		       "Fake semaphore does not support context switch.");
	--sem->n;
}

bool cn_sem_trywait(CnSem* sem)
{
	ENSURE(sem, ECODES.null_param);
	if (sem->n) {
		--sem->n;
		return true;
	}
	return false;
}

void cn_sem_post(CnSem* sem)
{
	ENSURE(sem, ECODES.null_param);
	++sem->n;
}
