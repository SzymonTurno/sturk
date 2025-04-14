#ifndef _UB_OS_MUTEX_H
#define _UB_OS_MUTEX_H

#include "ub/os/mutex.h"

#define MUTEX_POLICY_MASK UB_MUTEX_POLICY_MASK
#define MUTEX_TYPE_MASK   UB_MUTEX_TYPE_MASK
#define MUTEX_UNUSED_MASK UB_MUTEX_UNUSED_MASK

#define MUTEX_BF UB_MUTEX_BF

#define MUTEX_POLICY_NONE         UB_MUTEX_POLICY_NONE
#define MUTEX_POLICY_PRIO_INHERIT UB_MUTEX_POLICY_PRIO_INHERIT
#define MUTEX_POLICY_DELIM        UB_MUTEX_POLICY_DELIM

#define MUTEX_TYPE_NONE      UB_MUTEX_TYPE_NONE
#define MUTEX_TYPE_RECURSIVE UB_MUTEX_TYPE_RECURSIVE
#define MUTEX_TYPE_DELIM     UB_MUTEX_TYPE_DELIM

#define mutex_create ub_mutex_create

#define mutex_destroy ub_mutex_destroy

#define mutex_lock ub_mutex_lock

#define mutex_trylock ub_mutex_trylock

#define mutex_unlock ub_mutex_unlock

#endif /* _UB_OS_MUTEX_H */
