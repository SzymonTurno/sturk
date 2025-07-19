/**
 * @file cn/os/sem.h
 * @brief Header file for semaphore.
 *
 * *** todo ***.
 */

#ifndef CN_OS_SEM_H
#define CN_OS_SEM_H

#include "cn/bits.h"
#include <stdbool.h>

typedef struct CnSem CnSem;

CnSem* cn_sem_create(CnBits args);

void cn_sem_destroy(CnSem* sem);

void cn_sem_wait(CnSem* sem);

bool cn_sem_trywait(CnSem* sem);

void cn_sem_post(CnSem* sem);

#endif /* CN_OS_SEM_H */
