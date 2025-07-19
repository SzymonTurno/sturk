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

/**
 * @var typedef struct CnSem CnSem
 * @brief *** todo ***.
 */
typedef struct CnSem CnSem;

/**
 * @fn CnSem* cn_sem_create(CnBits args)
 * @brief *** todo ***.
 * @param[in] args Input.
 * @returns *** todo ***.
 */
CnSem* cn_sem_create(CnBits args);

/**
 * @fn void cn_sem_destroy(CnSem* sem)
 * @brief *** todo ***.
 * @param[in,out] sem Input/output.
 */
void cn_sem_destroy(CnSem* sem);

/**
 * @fn void cn_sem_wait(CnSem* sem)
 * @brief *** todo ***.
 * @param[in,out] sem Input/output.
 */
void cn_sem_wait(CnSem* sem);

/**
 * @fn bool cn_sem_trywait(CnSem* sem)
 * @brief *** todo ***.
 * @param[in,out] sem Input/output.
 * @returns *** todo ***.
 */
bool cn_sem_trywait(CnSem* sem);

/**
 * @fn void cn_sem_post(CnSem* sem)
 * @brief *** todo ***.
 * @param[in,out] sem Input/output.
 */
void cn_sem_post(CnSem* sem);

#endif /* CN_OS_SEM_H */
