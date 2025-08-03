/**
 * @file cn/os/sem.h
 *
 * @brief Semaphore.
 */

#ifndef CN_OS_SEM_H
#define CN_OS_SEM_H

#include "cn/bits.h"
#include <stdbool.h>

/**
 * @var typedef struct CnSem CnSem
 *
 * @brief Semaphore.
 */
typedef struct CnSem CnSem;

/**
 * @fn CnSem* cn_sem_create(CnBits args)
 *
 * @brief Create a semaphore.
 *
 * @param[in] args The configuration arguments.
 *
 * @return The new semaphore.
 */
CnSem* cn_sem_create(CnBits args);

/**
 * @fn void cn_sem_destroy(CnSem* sem)
 *
 * @brief Destroy a semaphore.
 *
 * @param[in,out] sem The semaphore.
 */
void cn_sem_destroy(CnSem* sem);

/**
 * @fn void cn_sem_wait(CnSem* sem)
 *
 * @brief Wait on semaphore.
 *
 * Blocks the calling thread until the semaphore counter is 0. Decrement the
 * counter.
 *
 * @param[in,out] sem The semaphore.
 */
void cn_sem_wait(CnSem* sem);

/**
 * @fn bool cn_sem_trywait(CnSem* sem)
 *
 * @brief Decrement the semaphore counter if it is greater than 0.
 *
 * @param[in,out] sem The semaphore.
 *
 * Does not block the calling thread.
 *
 * @return True, if the counter has been successfully decremented.
 */
bool cn_sem_trywait(CnSem* sem);

/**
 * @fn void cn_sem_post(CnSem* sem)
 *
 * @brief Increment the semaphore counter.
 *
 * @param[in,out] sem The semaphore.
 */
void cn_sem_post(CnSem* sem);

#endif /* CN_OS_SEM_H */
