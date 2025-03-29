#ifndef UB_ARITH_H
#define UB_ARITH_H

#include <stddef.h>

#define ub_max(x, y)                                                          \
	({                                                                     \
		__typeof__(x) _x = (x);                                        \
		__typeof__(y) _y = (y);                                        \
									       \
		(_x > _y) ? _x : _y;                                           \
	})

#define ub_min(x, y)                                                          \
	({                                                                     \
		__typeof__(x) _x = (x);                                        \
		__typeof__(y) _y = (y);                                        \
									       \
		(_x < _y) ? _x : _y;                                           \
	})

#define ub_container_of(ptr, type, member)                                    \
	({                                                                     \
		const __typeof__(((type*)0)->member) *_ptr = (ptr);            \
									       \
		(type*)((char*)_ptr - offsetof(type, member));                 \
	})

#endif /* UB_ARITH_H */
