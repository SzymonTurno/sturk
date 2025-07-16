/**
 * \file cn/os/mutex.h
 * \brief Header file for mutex.
 *
 * *** todo ***.
 */

#ifndef CN_OS_MUTEX_H
#define CN_OS_MUTEX_H

#include "cn/bits.h"
#include <stdbool.h>

#define CN_MUTEX_POLICY_MASK CN_BITMASK(2, 0)
#define CN_MUTEX_TYPE_MASK   CN_BITMASK(6, 3)
#define CN_MUTEX_UNUSED_MASK CN_BITMASK(31, 7)

#define CN_MUTEX_BF(opt, val) CN_BITFIELD(CN_MUTEX_##opt##_MASK, (val))

#define CN_MUTEX_POLICY_NONE         CN_MUTEX_BF(POLICY, 0)
#define CN_MUTEX_POLICY_PRIO_INHERIT CN_MUTEX_BF(POLICY, 1)
#define CN_MUTEX_POLICY_DELIM        CN_MUTEX_BF(POLICY, 8)

#define CN_MUTEX_TYPE_NONE      CN_MUTEX_BF(TYPE, 0)
#define CN_MUTEX_TYPE_RECURSIVE CN_MUTEX_BF(TYPE, 1)
#define CN_MUTEX_TYPE_DELIM     CN_MUTEX_BF(TYPE, 16)

typedef struct CnMutex CnMutex;

CnMutex* cn_mutex_create(CnBits args);

void cn_mutex_destroy(CnMutex* mutex);

void cn_mutex_lock(CnMutex* mutex);

bool cn_mutex_trylock(CnMutex* mutex);

void cn_mutex_unlock(CnMutex* mutex);

#endif /* CN_OS_MUTEX_H */
