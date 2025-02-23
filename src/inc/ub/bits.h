#ifndef UB_BITS_H
#define UB_BITS_H

#define ub_bf_lsb(bits) (__builtin_ffs(bits) - 1)

#define ub_bf_set(bits, mask, val) (((bits) & ~(mask)) | (val))

#define UBit(pos) (1 << (pos))

#define UBitmask(hi, lo) ((1 << hi) * 2 - (1 << lo))

#define UBitfield(mask, val) ((val) << ub_bf_lsb(mask))

typedef int UBits;

#endif /* UB_BITS_H */
