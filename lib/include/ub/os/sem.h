#ifndef UB_OS_SEM_H
#define UB_OS_SEM_H

#include "ub/bits.h"

typedef struct UBsem UBsem;

enum UBsemStatus {
	UB_SEM_OK = 0,
	UB_SEM_FAIL
};

UBsem* ub_sem_create(UBits args);

void ub_sem_destroy(UBsem* sem);

enum UBsemStatus ub_sem_wait(UBsem* sem);

enum UBsemStatus ub_sem_post(UBsem* sem);

#endif /* UB_OS_SEM_H */
