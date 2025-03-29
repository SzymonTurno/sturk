#ifndef UB_OS_MUTEX_H
#define UB_OS_MUTEX_H

#include "ub/bits.h"

#define UB_MUTEX_PROTOCOL_MASK UBitmask(2, 0)
#define UB_MUTEX_TYPE_MASK     UBitmask(6, 3)
#define UB_MUTEX_UNUSED_MASK   UBitmask(31, 7)

#define UB_MUTEX_BF(opt, val) UBitfield(UB_MUTEX_##opt##_MASK, (val))

enum UBmutexProtocol {
	UB_MUTEX_PROTOCOL_NONE         = UB_MUTEX_BF(PROTOCOL, 0),
	UB_MUTEX_PROTOCOL_PRIO_INHERIT = UB_MUTEX_BF(PROTOCOL, 1),
	_UB_MUTEX_PROTOCOL_DELIM       = UB_MUTEX_BF(PROTOCOL, 8)
};

enum UBmutexType {
	UB_MUTEX_TYPE_NONE      = UB_MUTEX_BF(TYPE, 0),
	UB_MUTEX_TYPE_RECURSIVE = UB_MUTEX_BF(TYPE, 1),
	_UB_MUTEX_TYPE_DELIM    = UB_MUTEX_BF(TYPE, 16)
};

typedef struct UBmutex UBmutex;

enum UBmutexStatus {
	UB_MUTEX_OK = 0,
	UB_MUTEX_DENY,
	UB_MUTEX_FAIL
};

UBmutex* ub_mutex_create(UBits args);

void ub_mutex_destroy(UBmutex* mutex);

enum UBmutexStatus ub_mutex_lock(UBmutex* mutex);

enum UBmutexStatus ub_mutex_trylock(UBmutex* mutex);

enum UBmutexStatus ub_mutex_unlock(UBmutex* mutex);

#endif /* UB_OS_MUTEX_H */
