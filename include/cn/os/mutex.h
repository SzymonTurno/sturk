/**
 * @file cn/os/mutex.h
 * @brief Header file for mutex.
 *
 * *** todo ***.
 */

#ifndef CN_OS_MUTEX_H
#define CN_OS_MUTEX_H

#include "cn/bits.h"
#include <stdbool.h>

/**
 * @def CN_MUTEX_BF(opt, val)
 * @brief *** todo ***.
 * @param[in] opt Input.
 * @param[in] val Input.
 * @returns *** todo ***.
 */
#define CN_MUTEX_BF(opt, val) CN_BITFIELD(CN_MUTEX_##opt##_MASK, (val))

/* clang-format off */
#define CN_MUTEX_POLICY_MASK CN_BITMASK(2, 0)  /**< mutex policy setting bits. */
#define CN_MUTEX_TYPE_MASK   CN_BITMASK(6, 3)  /**< mutex type setting bits. */
#define CN_MUTEX_UNUSED_MASK CN_BITMASK(31, 7) /**< mutex settings unused bits. */

#define CN_MUTEX_POLICY_NONE         CN_MUTEX_BF(POLICY, 0) /**< mutex policy none. */
#define CN_MUTEX_POLICY_PRIO_INHERIT CN_MUTEX_BF(POLICY, 1) /**< mutex policy priority inherit. */
#define CN_MUTEX_POLICY_DELIM        CN_MUTEX_BF(POLICY, 8) /**< mutex policy deliminator. */

#define CN_MUTEX_TYPE_NONE      CN_MUTEX_BF(TYPE, 0)  /**< mutex type none. */
#define CN_MUTEX_TYPE_RECURSIVE CN_MUTEX_BF(TYPE, 1)  /**< mutex type recursive. */
#define CN_MUTEX_TYPE_DELIM     CN_MUTEX_BF(TYPE, 16) /**< mutex type deliminator. */
/* clang-format on */

/**
 * @var typedef struct CnMutex CnMutex
 * @brief *** todo ***.
 */
typedef struct CnMutex CnMutex;

/**
 * @fn CnMutex* cn_mutex_create(CnBits args)
 * @brief *** todo ***.
 * @param[in] args Input.
 * @returns *** todo ***.
 */
CnMutex* cn_mutex_create(CnBits args);

/**
 * @fn void cn_mutex_destroy(CnMutex* mutex)
 * @brief *** todo ***.
 * @param[in,out] mutex Input/output.
 */
void cn_mutex_destroy(CnMutex* mutex);

/**
 * @fn void cn_mutex_lock(CnMutex* mutex)
 * @brief *** todo ***.
 * @param[in,out] mutex Input/output.
 */
void cn_mutex_lock(CnMutex* mutex);

/**
 * @fn bool cn_mutex_trylock(CnMutex* mutex)
 * @brief *** todo ***.
 * @param[in,out] mutex Input/output.
 * @returns *** todo ***.
 */
bool cn_mutex_trylock(CnMutex* mutex);

/**
 * @fn bool cn_mutex_unlock(CnMutex* mutex)
 * @brief *** todo ***.
 * @param[in,out] mutex Input/output.
 */
void cn_mutex_unlock(CnMutex* mutex);

#endif /* CN_OS_MUTEX_H */
