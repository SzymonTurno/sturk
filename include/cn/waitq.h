/**
 * @file cn/waitq.h
 *
 * @brief Waiting queue.
 */

#ifndef CN_WAITQ_H
#define CN_WAITQ_H

#include "cn/cirq.h"

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
 * @fn void cn_waitq_ins(CnWaitq* waitq, struct CnBinode* entry)
 *
 * @brief Insert an entry into a waiting queue
 *
 * @param[in,out] waitq The waiting queue.
 * @param[in,out] entry The inserted entry.
 */
void cn_waitq_ins(CnWaitq* waitq, struct CnBinode* entry);

/**
 * @fn struct CnBinode* cn_waitq_rem(CnWaitq* waitq)
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
struct CnBinode* cn_waitq_rem(CnWaitq* waitq);

/**
 * @fn struct CnBinode* cn_waitq_tryrem(CnWaitq* waitq)
 *
 * @brief Try to remove an entry from the front of the queue.
 *
 * @param[in,out] waitq The waiting queue.
 *
 * This will not block the thread.
 *
 * @return The removed entry if the queue was not empty. Otherwise, NULL.
 */
struct CnBinode* cn_waitq_tryrem(CnWaitq* waitq);

#endif /* CN_WAITQ_H */
