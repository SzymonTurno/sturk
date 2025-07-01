#ifndef CN_ARITH_H
#define CN_ARITH_H

#include <stddef.h>

#ifdef __STRICT_ANSI__

#define cn_max(x, y) (((x) > (y)) ? (x) : (y))

#define cn_min(x, y) (((x) < (y)) ? (x) : (y))

/* clang-format off */
#define cn_container_of(ptr, type, member)                                     \
	((type*)((char*)(ptr) - offsetof(type, member)))
/* clang-format on */

#else /* __STRICT_ANSI__ */

#define cn_max(x, y)                                                           \
	({                                                                     \
		__typeof__(x) _x = (x);                                        \
		__typeof__(y) _y = (y);                                        \
                                                                               \
		(_x > _y) ? _x : _y;                                           \
	})

#define cn_min(x, y)                                                           \
	({                                                                     \
		__typeof__(x) _x = (x);                                        \
		__typeof__(y) _y = (y);                                        \
                                                                               \
		(_x < _y) ? _x : _y;                                           \
	})

#define cn_container_of(ptr, type, member)                                     \
	({                                                                     \
		const __typeof__(((type*)0)->member)* _ptr = (ptr);            \
                                                                               \
		(type*)((char*)_ptr - offsetof(type, member));                 \
	})

#endif /* __STRICT_ANSI__ */

#endif /* CN_ARITH_H */
