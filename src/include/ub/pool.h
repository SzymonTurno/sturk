#ifndef UB_POOL_H
#define UB_POOL_H

#include <stddef.h>

typedef struct UBpool UBpool;

UBpool* ub_pool_create(size_t blk_size);

void ub_pool_destroy(UBpool* pool);

void* ub_pool_alloc(UBpool* pool);

void* ub_pool_tryalloc(UBpool* pool);

void ub_pool_free(UBpool* pool, void* blk);

#endif /* UB_POOL_H */
