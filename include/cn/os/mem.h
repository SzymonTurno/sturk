/**
 * @file cn/os/mem.h
 * @brief Header file for memory allocator.
 *
 * *** todo ***.
 */

#ifndef CN_OS_MEM_H
#define CN_OS_MEM_H

#include <stddef.h>

/* @cond */
#define _CN_NEW(type, n, ...) ((type*)cn_malloc(sizeof(type) * n))
/* @endcond */

/**
 * @def CN_NEW(...)
 * @brief *** todo ***.
 * @param[in] ... (1) type input; (2) n (optional) input, 1 by default.
 * @returns *** todo ***.
 */
#define CN_NEW(...) _CN_NEW(__VA_ARGS__, 1, )

/**
 * @fn void* cn_malloc(size_t size)
 * @brief *** todo ***.
 * @param[in] size Input.
 * @returns *** todo ***.
 */
void* cn_malloc(size_t size);

/**
 * @fn void cn_free(void* ptr)
 * @brief *** todo ***.
 * @param[in,out] ptr Input.
 */
void cn_free(void* ptr);

#endif /* CN_OS_MEM_H */
