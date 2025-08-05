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
 * @file cn/os/mutex.h
 *
 * @brief Mutex.
 */

#ifndef CN_OS_MUTEX_H
#define CN_OS_MUTEX_H

#include "cn/bits.h"
#include <stdbool.h>

/**
 * @def CN_MUTEX_BF(opt, val)
 *
 * @brief Create a bitfield for the mutex attribute option.
 *
 * @param[in] attr The attribute: POLICY or TYPE.
 * @param[in] opt The unique integer identifier for the option.
 *
 * @return The bitfield.
 */
#define CN_MUTEX_BF(attr, opt) CN_BITFIELD(CN_MUTEX_##attr##_MASK, (opt))

/* clang-format off */
#define CN_MUTEX_POLICY_MASK CN_BITMASK(2, 0)  /**< mutex policy setting bits. */
#define CN_MUTEX_TYPE_MASK   CN_BITMASK(6, 3)  /**< mutex type setting bits. */
#define CN_MUTEX_UNUSED_MASK CN_BITMASK(31, 7) /**< mutex settings unused bits. */

#define CN_MUTEX_POLICY_NONE         CN_MUTEX_BF(POLICY, 0) /**< mutex policy none. */
#define CN_MUTEX_POLICY_PRIO_INHERIT CN_MUTEX_BF(POLICY, 1) /**< mutex policy priority inherit. */
#define CN_MUTEX_POLICY_DELIM        CN_MUTEX_BF(POLICY, 8) /**< mutex policy deliminator. */

#define CN_MUTEX_TYPE_NONE      CN_MUTEX_BF(TYPE, 0)  /**< mutex type none. */
#define CN_MUTEX_TYPE_RECURSIVE CN_MUTEX_BF(TYPE, 1)  /**< mutex type recursive. */
#define CN_MUTEX_TYPE_DELIM     CN_MUTEX_BF(TYPE, 16) /**< mutex type deliminator. */
/* clang-format on */

/**
 * @var typedef struct CnMutex CnMutex
 *
 * @brief Mutex.
 */
typedef struct CnMutex CnMutex;

/**
 * @fn CnMutex* cn_mutex_create(CnBits args)
 *
 * @brief Create a mutex.
 *
 * @param[in] args The configuration arguments.
 *
 * Create a recursive mutex with a priority inherit policy.
 * @code
 * m = cn_mutex_create(CN_MUTEX_POLICY_PRIO_INHERIT | CN_MUTEX_TYPE_RECURSIVE);
 * @endcode
 *
 * @return The new mutex.
 */
CnMutex* cn_mutex_create(CnBits args);

/**
 * @fn void cn_mutex_destroy(CnMutex* mutex)
 *
 * @brief Destroy a mutex.
 *
 * @param[in,out] mutex The mutex.
 */
void cn_mutex_destroy(CnMutex* mutex);

/**
 * @fn void cn_mutex_lock(CnMutex* mutex)
 *
 * @brief Block the calling thread until the mutex is unlocked. Lock the mutex.
 *
 * @param[in,out] mutex The mutex.
 */
void cn_mutex_lock(CnMutex* mutex);

/**
 * @fn bool cn_mutex_trylock(CnMutex* mutex)
 *
 * @brief Lock the mutex if it is not locked. Do not block the calling thread.
 *
 * @param[in,out] mutex The mutex.
 *
 * @return True, if the mutex has been successfully locked.
 */
bool cn_mutex_trylock(CnMutex* mutex);

/**
 * @fn bool cn_mutex_unlock(CnMutex* mutex)
 *
 * @brief Unlock a mutex.
 *
 * @param[in,out] mutex The mutex.
 */
void cn_mutex_unlock(CnMutex* mutex);

#endif /* CN_OS_MUTEX_H */
