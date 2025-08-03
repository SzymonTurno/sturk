/**
 * @file cantil/waitq.h
 *
 * @see cn/waitq.h
 */

#ifndef CANTIL_WAITQ_H
#define CANTIL_WAITQ_H

#include "cn/waitq.h"

/** @see cn_waitq_create() */
#define waitq_create cn_waitq_create

/** @see cn_waitq_destroy() */
#define waitq_destroy cn_waitq_destroy

/** @see cn_waitq_ins() */
#define waitq_ins cn_waitq_ins

/** @see cn_waitq_rem() */
#define waitq_rem cn_waitq_rem

/** @see cn_waitq_tryrem() */
#define waitq_tryrem cn_waitq_tryrem

#endif /* CANTIL_WAITQ_H */
