/**
 * @file cn/bits.h
 *
 * @brief Bits and bitfields.
 *
 * This header file provides macros for operating with bits and bitfields.
 * Use CnBits data type for bitwise operations as it avoids many issues related
 * to integral promotion, which is a common source of bugs when doing bit
 * manipulations.
 *
 * @note Compiling with -Wconversion is recommended.
 *
 * Usage.
 *   1. Set.
 * @code
 *     word |= CN_BIT(pos);
 *     word |= CN_BITMASK(hi, lo);
 * @endcode
 *
 *   2. Clear.
 * @code
 *     word &= ~CN_BIT(pos);
 *     word $= ~CN_BITMASK(hi, lo);
 * @endcode
 */

#ifndef CN_BITS_H
#define CN_BITS_H

/**
 * @def cn_bf_set(word, mask, val)
 *
 * @brief Set bitfield in a word to a new value.
 *
 * @param[in] word The word that contains the bitfield.
 * @param[in] mask The bitmask of the bitfield.
 * @param[in] val The new value of the bitfield.
 *
 * @return New value of the word.
 */
#define cn_bf_set(word, mask, val) (((word) & ~(mask)) | (val))

/**
 * @def CN_BIT(pos)
 *
 * @brief Create a bitmask for a single bit.
 *
 * This macro creates a bitmask for a bit at position @a pos.
 *
 * @return Bitmask for a bit at @a pos position.
 */
#define CN_BIT(pos) (1 << (pos))

/**
 * @def CN_BITMASK(hi, lo)
 *
 * @brief Create a continuous bitmask.
 *
 * This macro creates a continous bitmask spannig from @a hi (most
 * significant bit) to @a lo (least significant bit).
 *
 * @return Continous bitmask spannig from @a hi to @a lo.
 */
#define CN_BITMASK(hi, lo) ((1 << (hi)) * 2 - (1 << (lo)))

/**
 * @def CN_BITFIELD(mask, val)
 *
 * @brief Create a bitfield.
 *
 * @param[in] mask The bitmask of the bitfield.
 * @param[in] val The value of the bitfield.
 *
 * @return Bitfield with value @a val at the position of the @a mask.
 */
#define CN_BITFIELD(mask, val) ((val) << (__builtin_ffs(mask) - 1))

/**
 * @var typedef int CnBits
 *
 * @brief Data type for bitwise operations.
 */
typedef int CnBits;

#endif /* CN_BITS_H */
