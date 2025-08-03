/**
 * @file cn/pool.h
 *
 * @brief Memory pool.
 *
 * Glossary
 * --------
 *
 * | Term | Description                   |
 * | ---- | ----------------------------- |
 * | pool | fixed-size blocks memory pool |
 */

#ifndef CN_POOL_H
#define CN_POOL_H

#include <stddef.h>

/**
 * @var typedef struct CnPool CnPool
 *
 * @brief *pool*.
 */
typedef struct CnPool CnPool;

/**
 * @fn CnPool* cn_pool_create(size_t blk_size)
 *
 * @brief Create a *pool*.
 *
 * @param[in] blk_size The size of the blocks.
 *
 * @return A new *pool*.
 */
CnPool* cn_pool_create(size_t blk_size);

/**
 * @fn void cn_pool_destroy(CnPool* pool)
 *
 * @brief Destroy a *pool*.
 *
 * @param[in,out] pool The *pool*.
 */
void cn_pool_destroy(CnPool* pool);

/**
 * @fn void* cn_pool_alloc(CnPool* pool)
 *
 * @brief Allocate a block from a *pool*.
 *
 * @param[in,out] pool The *pool*.
 *
 * If the *pool* is empty, allocates a new block from the system memory
 * allocator.
 * @see cn/os/mem.h
 *
 * @return The block.
 */
void* cn_pool_alloc(CnPool* pool);

/**
 * @fn void* cn_pool_tryalloc(CnPool* pool)
 *
 * @brief Try to allocate a block from a *pool*.
 *
 * @param[in,out] pool The *pool*.
 *
 * @return The block, if the *pool* is not empty. Otherwise, NULL.
 */
void* cn_pool_tryalloc(CnPool* pool);

/**
 * @fn void cn_pool_free(CnPool* pool, void* blk)
 *
 * @brief Return a block to a *pool*.
 *
 * @param[in,out] pool The *pool*.
 * @param[in,out] blk The block.
 */
void cn_pool_free(CnPool* pool, void* blk);

#endif /* CN_POOL_H */
