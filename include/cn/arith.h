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
