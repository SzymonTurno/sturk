#ifndef UB_DEBUG_H
#define UB_DEBUG_H

#include "ub/logger.h"
#include "ub/os/mem.h"

#define ub_ensure(cond, text)                                                 \
	do {                                                                   \
		if (!cond) {                                                   \
			ub_log_error(text);                                    \
			ub_exit();                                             \
		}                                                              \
	} while (0)

#define UB_ENSURE(cond, text)                                                 \
	do {                                                                   \
		if (!cond) {                                                   \
			UB_LOG_ERROR(text);                                    \
			ub_exit();                                             \
		}                                                              \
	} while (0)

#define ub_raise(text) ub_ensure(0, text)

#define UB_RAISE(text) UB_ENSURE(0, text)

#endif /* UB_DEBUG_H */
