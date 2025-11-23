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
 * @file st/os/mutex.h
 *
 * @brief Mutex.
 */

#ifndef ST_OS_MUTEX_H
#define ST_OS_MUTEX_H

#include "vertegs/bits.h"

/**
 * @def ST_MUTEX_BF(opt, val)
 *
 * @brief Create a bitfield for the mutex attribute option.
 *
 * @param[in] attr The attribute: POLICY or TYPE.
 * @param[in] opt The unique integer identifier for the option.
 *
 * @return The bitfield.
 */
#define ST_MUTEX_BF(attr, opt) VX_BITFIELD(ST_MUTEX_##attr##_MASK, (opt))

/* clang-format off */
#define ST_MUTEX_POLICY_MASK VX_BITMASK(2, 0)  /**< mutex policy setting bits. */
#define ST_MUTEX_TYPE_MASK   VX_BITMASK(6, 3)  /**< mutex type setting bits. */
#define ST_MUTEX_UNUSED_MASK VX_BITMASK(31, 7) /**< mutex settings unused bits. */

#define ST_MUTEX_POLICY_NONE         ST_MUTEX_BF(POLICY, 0) /**< mutex policy none. */
#define ST_MUTEX_POLICY_PRIO_INHERIT ST_MUTEX_BF(POLICY, 1) /**< mutex policy priority inherit. */
#define ST_MUTEX_POLICY_DELIM        ST_MUTEX_BF(POLICY, 8) /**< mutex policy deliminator. */

#define ST_MUTEX_TYPE_NONE      ST_MUTEX_BF(TYPE, 0)  /**< mutex type none. */
#define ST_MUTEX_TYPE_RECURSIVE ST_MUTEX_BF(TYPE, 1)  /**< mutex type recursive. */
#define ST_MUTEX_TYPE_DELIM     ST_MUTEX_BF(TYPE, 16) /**< mutex type deliminator. */
/* clang-format on */

/**
 * @var typedef struct StMutex StMutex
 *
 * @brief Mutex.
 */
typedef struct StMutex StMutex;

/**
 * @fn StMutex* st_mutex_create(VxBits args)
 *
 * @brief Create a mutex.
 *
 * @param[in] args The configuration arguments.
 *
 * Create a recursive mutex with a priority inherit policy.
 * @code
 * m = st_mutex_create(ST_MUTEX_POLICY_PRIO_INHERIT | ST_MUTEX_TYPE_RECURSIVE);
 * @endcode
 *
 * @return The new mutex.
 */
StMutex* st_mutex_create(VxBits args);

/**
 * @fn void st_mutex_destroy(StMutex* mutex)
 *
 * @brief Destroy a mutex.
 *
 * @param[in,out] mutex The mutex.
 */
void st_mutex_destroy(StMutex* mutex);

/**
 * @fn void st_mutex_lock(StMutex* mutex)
 *
 * @brief Block the calling thread until the mutex is unlocked. Lock the mutex.
 *
 * @param[in,out] mutex The mutex.
 */
void st_mutex_lock(StMutex* mutex);

/**
 * @fn int st_mutex_trylock(StMutex* mutex)
 *
 * @brief Lock the mutex if it is not locked. Do not block the calling thread.
 *
 * @param[in,out] mutex The mutex.
 *
 * @return True, if the mutex has been successfully locked.
 */
int st_mutex_trylock(StMutex* mutex);

/**
 * @fn bool st_mutex_unlock(StMutex* mutex)
 *
 * @brief Unlock a mutex.
 *
 * @param[in,out] mutex The mutex.
 */
void st_mutex_unlock(StMutex* mutex);

#endif /* ST_OS_MUTEX_H */
