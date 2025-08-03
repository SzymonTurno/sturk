/**
 * @file cn/os/mem.h
 *
 * @brief Memory allocator.
 */

#ifndef CN_OS_MEM_H
#define CN_OS_MEM_H

#include <stddef.h>

/* @cond */
#define _CN_NEW(type, n, ...) ((type*)cn_malloc(sizeof(type) * n))
/* @endcond */

/**
 * @def CN_NEW(...)
 *
 * @brief Allocate memory for a data type.
 *
 * A call CN_NEW(type, n) will alocate contiguous memory region of the length
 * that is equal to the multiple of @a n and the size of @a type. The @a n
 * argument is optional and by default it equals 1.
 */
#define CN_NEW(...) _CN_NEW(__VA_ARGS__, 1, )

/**
 * @fn void* cn_malloc(size_t size)
 *
 * @see malloc()
 */
void* cn_malloc(size_t size);

/**
 * @fn void cn_free(void* ptr)
 *
 * @see free()
 */
void cn_free(void* ptr);

#endif /* CN_OS_MEM_H */
