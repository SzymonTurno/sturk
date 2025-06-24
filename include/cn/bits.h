#ifndef CN_BITS_H
#define CN_BITS_H

#define cn_bf_lsb(bits) (__builtin_ffs(bits) - 1)

#define cn_bf_set(bits, mask, val) (((bits) & ~(mask)) | (val))

#define CN_BIT(pos) (1 << (pos))

#define CN_BITMASK(hi, lo) ((1 << hi) * 2 - (1 << lo))

#define CN_BITFIELD(mask, val) ((val) << cn_bf_lsb(mask))

typedef int CnBits;

#endif /* CN_BITS_H */
