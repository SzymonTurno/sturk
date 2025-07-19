/**
 * @file cn/waitq.h
 * @brief Header file for waiting queue.
 *
 * *** todo ***.
 */

#ifndef CN_WAITQ_H
#define CN_WAITQ_H

#include "cn/cirq.h"

typedef struct CnWaitq CnWaitq;

CnWaitq* cn_waitq_create(void);

void cn_waitq_destroy(CnWaitq* waitq);

void cn_waitq_ins(CnWaitq* waitq, struct CnBinode* entry);

struct CnBinode* cn_waitq_rem(CnWaitq* waitq);

struct CnBinode* cn_waitq_tryrem(CnWaitq* waitq);

#endif /* CN_WAITQ_H */
