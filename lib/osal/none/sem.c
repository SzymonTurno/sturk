#include "ub/os/sem.h"
#include <stddef.h>

UBsem* ub_sem_create(UBits args)
{
	(void)args;
	return NULL;
}

void ub_sem_destroy(UBsem* sem)
{
	(void)sem;
}

void ub_sem_wait(UBsem* sem)
{
	(void)sem;
}

bool ub_sem_trywait(UBsem* sem)
{
	(void)sem;
	return true;
}

void ub_sem_post(UBsem* sem)
{
	(void)sem;
}
