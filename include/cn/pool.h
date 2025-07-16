/**
 * \file cn/pool.h
 * \brief Header file for memory pool.
 *
 * *** todo ***.
 */

#ifndef CN_POOL_H
#define CN_POOL_H

#include <stddef.h>

typedef struct CnPool CnPool;

CnPool* cn_pool_create(size_t blk_size);

void cn_pool_destroy(CnPool* pool);

void* cn_pool_alloc(CnPool* pool);

void* cn_pool_tryalloc(CnPool* pool);

void cn_pool_free(CnPool* pool, void* blk);

#endif /* CN_POOL_H */
