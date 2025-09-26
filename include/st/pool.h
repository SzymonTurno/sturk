/*
BSD 3-Clause License

Copyright (c) 2025, Szymon Turno

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/**
 * @file st/pool.h
 *
 * @brief Memory pool.
 */

#ifndef ST_POOL_H
#define ST_POOL_H

#include "st/os/sys.h"
#include "vertegs/list.h"
#include <stddef.h>

/**
 * @var typedef struct StPool StPool
 *
 * @brief *pool*.
 */
typedef struct StPool StPool;

/**
 * @def VX_LIST(union StFreeList, void*)
 *
 * @union StFreeList
 *
 * @brief Free list.
 */
VX_LIST(union StFreeList, void*);
/**< This is a macro definition of the free list used by the pool. */

/**
 * @fn StPool* st_pool_create(size_t blk_size)
 *
 * @brief Create a *pool*.
 *
 * @param[in] blk_size The size of the blocks.
 *
 * @return A new *pool*.
 */
StPool* st_pool_create(size_t blk_size);

/**
 * @fn void st_pool_destroy(StPool* pool)
 *
 * @brief Destroy a *pool*.
 *
 * @param[in,out] pool The *pool*.
 */
void st_pool_destroy(StPool* pool);

/**
 * @fn void* st_pool_alloc(StPool* pool)
 *
 * @brief Allocate a block from a *pool*.
 *
 * @param[in,out] pool The *pool*.
 *
 * If the *pool* is empty, allocates a new block from the system memory
 * allocator.
 * @see st/os/mem.h
 *
 * @return The block.
 */
void* st_pool_alloc(StPool* pool);

/**
 * @fn void* st_pool_tryalloc(StPool* pool)
 *
 * @brief Try to allocate a block from a *pool*.
 *
 * @param[in,out] pool The *pool*.
 *
 * @return The block, if the *pool* is not empty. Otherwise, NULL.
 */
void* st_pool_tryalloc(StPool* pool);

/**
 * @fn void st_pool_free(StPool* pool, void* blk)
 *
 * @brief Return a block to a *pool*.
 *
 * @param[in,out] pool The *pool*.
 * @param[in,out] blk The block.
 */
void st_pool_free(StPool* pool, void* blk);

#endif /* ST_POOL_H */
