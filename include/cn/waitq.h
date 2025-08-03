/**
 * @file cn/waitq.h
 * @brief Header file for waiting queue.
 *
 * *** todo ***.
 */

#ifndef CN_WAITQ_H
#define CN_WAITQ_H

#include "cn/cirq.h"

/**
 * @var typedef struct CnWaitq CnWaitq
 * @brief *** todo ***.
 */
typedef struct CnWaitq CnWaitq;

/**
 * @fn CnWaitq* cn_waitq_create(void)
 * @brief *** todo ***.
 * @return *** todo ***.
 */
CnWaitq* cn_waitq_create(void);

/**
 * @fn void cn_waitq_destroy(CnWaitq* waitq)
 * @brief *** todo ***.
 * @param[in,out] waitq Input/output.
 */
void cn_waitq_destroy(CnWaitq* waitq);

/**
 * @fn void cn_waitq_ins(CnWaitq* waitq, struct CnBinode* entry)
 * @brief *** todo ***.
 * @param[in,out] waitq Input/output.
 * @param[in,out] entry Input/output.
 */
void cn_waitq_ins(CnWaitq* waitq, struct CnBinode* entry);

/**
 * @fn struct CnBinode* cn_waitq_rem(CnWaitq* waitq)
 * @brief *** todo ***.
 * @param[in,out] waitq Input/output.
 * @return *** todo ***.
 */
struct CnBinode* cn_waitq_rem(CnWaitq* waitq);

/**
 * @fn struct CnBinode* cn_waitq_tryrem(CnWaitq* waitq)
 * @brief *** todo ***.
 * @param[in,out] waitq Input/output.
 * @return *** todo ***.
 */
struct CnBinode* cn_waitq_tryrem(CnWaitq* waitq);

#endif /* CN_WAITQ_H */
