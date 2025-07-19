/**
 * @file cn/pool.h
 * @brief Header file for memory pool.
 *
 * *** todo ***.
 */

#ifndef CN_POOL_H
#define CN_POOL_H

#include <stddef.h>

/**
 * @var typedef struct CnPool CnPool
 * @brief *** todo ***.
 */
typedef struct CnPool CnPool;

/**
 * @fn CnPool* cn_pool_create(size_t blk_size)
 * @brief *** todo ***.
 * @param[in] blk_size Input.
 * @returns *** todo ***.
 */
CnPool* cn_pool_create(size_t blk_size);

/**
 * @fn void cn_pool_destroy(CnPool* pool)
 * @brief *** todo ***.
 * @param[in,out] pool Input/output.
 */
void cn_pool_destroy(CnPool* pool);

/**
 * @fn void* cn_pool_alloc(CnPool* pool)
 * @brief *** todo ***.
 * @param[in,out] pool Input/output.
 * @returns *** todo ***.
 */
void* cn_pool_alloc(CnPool* pool);

/**
 * @fn void* cn_pool_tryalloc(CnPool* pool)
 * @brief *** todo ***.
 * @param[in,out] pool Input/output.
 * @returns *** todo ***.
 */
void* cn_pool_tryalloc(CnPool* pool);

/**
 * @fn void cn_pool_free(CnPool* pool, void* blk)
 * @brief *** todo ***.
 * @param[in,out] pool Input/output.
 * @param[in,out] blk Input/output.
 */
void cn_pool_free(CnPool* pool, void* blk);

#endif /* CN_POOL_H */
