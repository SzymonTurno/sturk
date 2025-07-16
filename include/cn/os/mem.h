/**
 * \file cn/os/mem.h
 * \brief Header file for memory allocator.
 *
 * *** todo ***.
 */

#ifndef CN_OS_MEM_H
#define CN_OS_MEM_H

#include <stddef.h>

#define _cn_new(type, n, ...) ((type*)cn_malloc(sizeof(type) * n))

#define cn_new(...) _cn_new(__VA_ARGS__, 1, )

void* cn_malloc(size_t size);

void cn_free(void* ptr);

#endif /* CN_OS_MEM_H */
