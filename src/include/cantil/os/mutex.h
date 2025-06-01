#ifndef CANTIL_OS_MUTEX_H
#define CANTIL_OS_MUTEX_H

#include "cn/os/mutex.h"

#define MUTEX_POLICY_MASK CN_MUTEX_POLICY_MASK
#define MUTEX_TYPE_MASK   CN_MUTEX_TYPE_MASK
#define MUTEX_UNUSED_MASK CN_MUTEX_UNUSED_MASK

#define MUTEX_BF CN_MUTEX_BF

#define MUTEX_POLICY_NONE         CN_MUTEX_POLICY_NONE
#define MUTEX_POLICY_PRIO_INHERIT CN_MUTEX_POLICY_PRIO_INHERIT
#define MUTEX_POLICY_DELIM        CN_MUTEX_POLICY_DELIM

#define MUTEX_TYPE_NONE      CN_MUTEX_TYPE_NONE
#define MUTEX_TYPE_RECURSIVE CN_MUTEX_TYPE_RECURSIVE
#define MUTEX_TYPE_DELIM     CN_MUTEX_TYPE_DELIM

#define mutex_create cn_mutex_create

#define mutex_destroy cn_mutex_destroy

#define mutex_lock cn_mutex_lock

#define mutex_trylock cn_mutex_trylock

#define mutex_unlock cn_mutex_unlock

#endif /* CANTIL_OS_MUTEX_H */
