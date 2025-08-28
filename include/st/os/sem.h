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
 * @file st/os/sem.h
 *
 * @brief Semaphore.
 */

#ifndef ST_OS_SEM_H
#define ST_OS_SEM_H

#include "st/bits.h"
#include <stdbool.h>

/**
 * @var typedef struct StSem StSem
 *
 * @brief Semaphore.
 */
typedef struct StSem StSem;

/**
 * @fn StSem* st_sem_create(StBits args)
 *
 * @brief Create a semaphore.
 *
 * @param[in] args The configuration arguments.
 *
 * @return The new semaphore.
 */
StSem* st_sem_create(StBits args);

/**
 * @fn void st_sem_destroy(StSem* sem)
 *
 * @brief Destroy a semaphore.
 *
 * @param[in,out] sem The semaphore.
 */
void st_sem_destroy(StSem* sem);

/**
 * @fn void st_sem_wait(StSem* sem)
 *
 * @brief Wait on semaphore.
 *
 * Blocks the calling thread until the semaphore counter is 0. Decrement the
 * counter.
 *
 * @param[in,out] sem The semaphore.
 */
void st_sem_wait(StSem* sem);

/**
 * @fn bool st_sem_trywait(StSem* sem)
 *
 * @brief Decrement the semaphore counter if it is greater than 0.
 *
 * @param[in,out] sem The semaphore.
 *
 * Does not block the calling thread.
 *
 * @return True, if the counter has been successfully decremented.
 */
bool st_sem_trywait(StSem* sem);

/**
 * @fn void st_sem_post(StSem* sem)
 *
 * @brief Increment the semaphore counter.
 *
 * @param[in,out] sem The semaphore.
 */
void st_sem_post(StSem* sem);

#endif /* ST_OS_SEM_H */
