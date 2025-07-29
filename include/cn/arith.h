/**
 * @file cn/arith.h
 *
 * @brief Basic arithmetic operations.
 */

#ifndef CN_ARITH_H
#define CN_ARITH_H

#include <stddef.h>

/**
 * @def CN_MAX(x, y)
 *
 * @brief Compute the maximum of two values.
 *
 * This macro returns the maximum of @a x and @a y.
 *
 * @return maximum of @a x and @a y.
 *
 * @note Side effects: @a x and @a y evaluated twice.
 */
#define CN_MAX(x, y) (((x) > (y)) ? (x) : (y))

/**
 * @def CN_MIN(x, y)
 *
 * @brief Compute the minimum of two values.
 *
 * This macro returns the minimum of @a x and @a y.
 *
 * @return minimum of @a x and @a y.
 *
 * @note Side effects: @a x and @a y evaluated twice.
 */
#define CN_MIN(x, y) (((x) < (y)) ? (x) : (y))

#ifdef __STRICT_ANSI__

/* clang-format off */
#define cn_container_of(ptr, type, member)                                     \
	((type*)((char*)(ptr) - offsetof(type, member)))
/* clang-format on */

#else /* not defined: __STRICT_ANSI__ */

/**
 * @def cn_container_of(ptr, type, member)
 *
 * @brief Cast a member of a structure out to the containing structure.
 *
 * @param[in] ptr The pointer to the member.
 * @param[in] type The type of the container struct this is embedded in.
 * @param[in] member The name of the member within the struct.
 *
 * @return Pointer to the member's container.
 *
 * @note Compile with the GNU extension to enable a type check for the @a ptr.
 */
#define cn_container_of(ptr, type, member)                                     \
	({                                                                     \
		const __typeof__(((type*)0)->member)* _ptr = (ptr);            \
                                                                               \
		(type*)((char*)_ptr - offsetof(type, member));                 \
	})

#endif /* __STRICT_ANSI__ */

#endif /* CN_ARITH_H */
