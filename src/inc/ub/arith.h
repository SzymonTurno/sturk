#ifndef UB_ARITH_H
#define UB_ARITH_H

#define ub_max(x, y)							       \
	({                                                                     \
		__typeof__(x) _x = (x);                                        \
		__typeof__(y) _y = (y);                                        \
									       \
		(_x > _y) ? _x : _y;                                           \
	})

#define ub_min(x, y)							       \
	({                                                                     \
		__typeof__(x) _x = (x);                                        \
		__typeof__(y) _y = (y);                                        \
									       \
		(_x < _y) ? _x : _y;                                           \
	})

#endif /* UB_ARITH_H */
