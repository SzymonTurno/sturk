#ifndef UB_OS_MUTEX_H
#define UB_OS_MUTEX_H

#include "ub/bits.h"
#include <stdbool.h>

#define UB_MUTEX_POLICY_MASK UBitmask(2, 0)
#define UB_MUTEX_TYPE_MASK   UBitmask(6, 3)
#define UB_MUTEX_UNUSED_MASK UBitmask(31, 7)

#define UB_MUTEX_BF(opt, val) UBitfield(UB_MUTEX_##opt##_MASK, (val))

enum UBmutexProtocol {
	UB_MUTEX_POLICY_NONE         = UB_MUTEX_BF(POLICY, 0),
	UB_MUTEX_POLICY_PRIO_INHERIT = UB_MUTEX_BF(POLICY, 1),
	_UB_MUTEX_POLICY_DELIM       = UB_MUTEX_BF(POLICY, 8)
};

enum UBmutexType {
	UB_MUTEX_TYPE_NONE      = UB_MUTEX_BF(TYPE, 0),
	UB_MUTEX_TYPE_RECURSIVE = UB_MUTEX_BF(TYPE, 1),
	_UB_MUTEX_TYPE_DELIM    = UB_MUTEX_BF(TYPE, 16)
};

typedef struct UBmutex UBmutex;

UBmutex* ub_mutex_create(UBits args);

void ub_mutex_destroy(UBmutex* mutex);

void ub_mutex_lock(UBmutex* mutex);

bool ub_mutex_trylock(UBmutex* mutex);

void ub_mutex_unlock(UBmutex* mutex);

#endif /* UB_OS_MUTEX_H */
