/*
BSD 3-Clause License

Copyright (c) 2025, Szymon Turno

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

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
