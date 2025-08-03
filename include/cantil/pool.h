/**
 * @file cantil/pool.h
 *
 * @see cn/pool.h
 */

#ifndef CANTIL_POOL_H
#define CANTIL_POOL_H

#include "cn/pool.h"

/** @see cn_pool_create() */
#define pool_create cn_pool_create

/** @see cn_pool_destroy() */
#define pool_destroy cn_pool_destroy

/** @see cn_pool_alloc() */
#define pool_alloc cn_pool_alloc

/** @see cn_pool_tryalloc() */
#define pool_tryalloc cn_pool_tryalloc

/** @see cn_pool_free() */
#define pool_free cn_pool_free

#endif /* CANTIL_POOL_H */
