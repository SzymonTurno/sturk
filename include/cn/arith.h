/**
 * @file cn/arith.h
 * @brief Header file for basic airthmetic operations.
 *
 * Compile without __STRICT_ANSI__ for additional type checks.
 */

#ifndef CN_ARITH_H
#define CN_ARITH_H

#include <stddef.h>

/**
 * @def cn_max(x, y)
 * @brief Computes the maximum of @a x and @a y.
 * @param[in] x Input.
 * @param[in] y Input.
 * @returns maximum of @a x and @a y.
 *
 * Side effects: @a x and @a y evaluated twice.
 */
#define cn_max(x, y) (((x) > (y)) ? (x) : (y))

/**
 * @def cn_min(x, y)
 * @brief Computes the minimum of @a x and @a y.
 * @param[in] x Input.
 * @param[in] y Input.
 * @returns minimum of @a and @a y.
 *
 * Side effects: @a x and @a y evaluated twice.
 */
#define cn_min(x, y) (((x) < (y)) ? (x) : (y))

#ifdef __STRICT_ANSI__

/* clang-format off */
#define cn_container_of(ptr, type, member)                                     \
	((type*)((char*)(ptr) - offsetof(type, member)))
/* clang-format on */

#else /* not defined: __STRICT_ANSI__ */

/**
 * @def cn_container_of(ptr, type, member)
 * @brief Computes container's address from @a ptr of container's @a member.
 * @param[in] ptr Input.
 * @param[in] type Input.
 * @param[in] member Input.
 * @returns pointer to the container instance of type @a type.
 *
 * No type check for @a ptr with __STRICT_ANSI__ build.
 */
#define cn_container_of(ptr, type, member)                                     \
	({                                                                     \
		const __typeof__(((type*)0)->member)* _ptr = (ptr);            \
                                                                               \
		(type*)((char*)_ptr - offsetof(type, member));                 \
	})

#endif /* __STRICT_ANSI__ */

#endif /* CN_ARITH_H */
