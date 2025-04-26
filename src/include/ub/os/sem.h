#ifndef UB_OS_SEM_H
#define UB_OS_SEM_H

#include "ub/bits.h"
#include <stdbool.h>

typedef struct UBsem UBsem;

UBsem* ub_sem_create(UBits args);

void ub_sem_destroy(UBsem* sem);

void ub_sem_wait(UBsem* sem);

bool ub_sem_trywait(UBsem* sem);

void ub_sem_post(UBsem* sem);

#endif /* UB_OS_SEM_H */
