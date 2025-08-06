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
 * @file cn/waitq.h
 *
 * @brief Waiting queue.
 */

#ifndef CN_WAITQ_H
#define CN_WAITQ_H

#include "cn/os/sys.h"
#include "vx/vertegs.h"

/**
 * @var typedef struct CnWaitq CnWaitq
 *
 * @brief Waiting queue.
 */
typedef struct CnWaitq CnWaitq;

/**
 * @fn CnWaitq* cn_waitq_create(void)
 *
 * @brief Create a new waiting queue.
 *
 * @return The new waiting queue.
 */
CnWaitq* cn_waitq_create(void);

/**
 * @fn void cn_waitq_destroy(CnWaitq* waitq)
 *
 * @brief Destroy a waiting queue.
 *
 * @param[in,out] waitq The waiting queue.
 */
void cn_waitq_destroy(CnWaitq* waitq);

/**
 * @fn void cn_waitq_ins(CnWaitq* waitq, struct Vertegs* entry)
 *
 * @brief Insert an entry into a waiting queue
 *
 * @param[in,out] waitq The waiting queue.
 * @param[in,out] entry The inserted entry.
 */
void cn_waitq_ins(CnWaitq* waitq, struct Vertegs* entry);

/**
 * @fn struct Vertegs* cn_waitq_rem(CnWaitq* waitq)
 *
 * @brief Remove an entry from the front of the queue.
 *
 * @param[in,out] waitq The waiting queue.
 *
 * This will block the thread if the queue is empty and wake it up only after an
 * entry has been inserted.
 *
 * @return The removed entry.
 */
struct Vertegs* cn_waitq_rem(CnWaitq* waitq);

/**
 * @fn struct Vertegs* cn_waitq_tryrem(CnWaitq* waitq)
 *
 * @brief Try to remove an entry from the front of the queue.
 *
 * @param[in,out] waitq The waiting queue.
 *
 * This will not block the thread.
 *
 * @return The removed entry if the queue was not empty. Otherwise, NULL.
 */
struct Vertegs* cn_waitq_tryrem(CnWaitq* waitq);

#endif /* CN_WAITQ_H */
