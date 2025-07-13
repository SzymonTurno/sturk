/**
 * \file cn/bits.h
 * \brief Header file for bitfield manipulations.
 *
 * *** todo ***.
 */

#ifndef CN_BITS_H
#define CN_BITS_H

/**
 * \def cn_bf_lsb(bits)
 * \brief *** todo ***.
 * \param[in] bits Input.
 * \returns *** todo ***.
 */
#define cn_bf_lsb(bits) (__builtin_ffs(bits) - 1)

/**
 * \def cn_bf_set(bits, mask, val)
 * \brief *** todo ***.
 * \param[in] bits Input.
 * \param[in] mask Input.
 * \param[in] val Input.
 * \returns *** todo ***.
 */
#define cn_bf_set(bits, mask, val) (((bits) & ~(mask)) | (val))

/**
 * \def CN_BIT(pos)
 * \brief *** todo ***.
 * \param[in] pos Input.
 * \returns *** todo ***.
 */
#define CN_BIT(pos) (1 << (pos))

/**
 * \def CN_BITMASK(hi, lo)
 * \brief *** todo ***.
 * \param[in] hi Input.
 * \param[in] lo Input.
 * \returns *** todo ***.
 */
#define CN_BITMASK(hi, lo) ((1 << hi) * 2 - (1 << lo))

/**
 * \def CN_BITFIELD(mask, val)
 * \brief *** todo ***.
 * \param[in] mask Input.
 * \param[in] val Input
 * \returns *** todo ***.
 */
#define CN_BITFIELD(mask, val) ((val) << cn_bf_lsb(mask))

/**
 * \var typedef int CnBits
 * \brief *** todo ***.
 */
typedef int CnBits;

#endif /* CN_BITS_H */
