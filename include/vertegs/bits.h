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
 * @file vertegs/bits.h
 *
 * @brief Bits and bitfields.
 *
 * This header file provides macros for operating with bits and bitfields.
 * Use the VxBits data type for bitwise operations as it avoids many issues
 * related to integral promotion, which is a common source of bugs when doing
 * bit manipulations.
 *
 * @note Compiling with -Wconversion is recommended.
 *
 * Usage.
 *   1. Set.
 * @code
 *     word |= VX_BIT(pos);
 *     word |= VX_BITMASK(hi, lo);
 * @endcode
 *
 *   2. Clear.
 * @code
 *     word &= ~VX_BIT(pos);
 *     word $= ~VX_BITMASK(hi, lo);
 * @endcode
 */

#ifndef VERTEGS_BITS_H
#define VERTEGS_BITS_H

/**
 * @file vertegs/bits.h
 *
 * @brief Vertex.
 *
 * This header file provides data types, functions and macros that define and
 * operate on graph vertices.
 */

/**
 * @def VX_BIT(pos)
 *
 * @brief Create a bitmask for a single bit.
 *
 * This macro creates a bitmask for a bit at position @a pos.
 *
 * @return A bitmask for the bit at the @a pos position.
 */
#define VX_BIT(pos) (1 << (pos))

/**
 * @def VX_BF_SET(word, mask, val)
 *
 * @brief Set a bitfield in a word to a new value.
 *
 * @param[in] word The word that contains the bitfield.
 * @param[in] mask The bitmask of the bitfield.
 * @param[in] val The new value of the bitfield.
 *
 * @return New value of the word.
 */
#define VX_BF_SET(word, mask, val) (((word) & ~(mask)) | ((mask) & (val)))

/**
 * @def VX_BITMASK(hi, lo)
 *
 * @brief Create a contiguous bitmask.
 *
 * This macro creates a contiguous bitmask that spans from the @a hi (most
 * significant bit) to the @a lo (least significant bit).
 *
 * @return A contiguous bitmask that spans from the @a hi to the @a lo.
 */
#define VX_BITMASK(hi, lo) ((1 << (hi)) * 2 - (1 << (lo)))

/**
 * @def VX_BITFIELD(mask, val)
 *
 * @brief Create a bitfield.
 *
 * @param[in] mask The bitmask of the bitfield.
 * @param[in] val The value of the bitfield.
 *
 * @return A bitfield with the value @a val at the position of the @a mask.
 */
#define VX_BITFIELD(mask, val) ((val) << (__builtin_ffs(mask) - 1))

/**
 * @var typedef int VxBits
 *
 * @brief The data type for bitwise operations.
 */
typedef int VxBits;

#endif /* VERTEGS_BITS_H */
