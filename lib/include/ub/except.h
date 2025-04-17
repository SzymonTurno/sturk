#ifndef UB_EXCEPT_H
#define UB_EXCEPT_H

#include "ub/logger.h"

struct UBexception {
	const char* reason;
};

static const struct {
	struct UBexception null_param[1];
	struct UBexception alloc_fail[1];
	struct UBexception sem_fail[1];
	struct UBexception mutex_fail[1];
} UB_ECODES = {
	.null_param = {{ "Null param." }},
	.alloc_fail = {{ "Memory allocation failed." }},
	.sem_fail = {{ "Semaphore failure." }},
	.mutex_fail = {{ "Mutex failure." }}
};

#define UB_ENSURE(cond, ecode)                                                \
	do {                                                                   \
		if (!cond) {                                                   \
			UB_LOG(UB_ERROR, NULL, "%s:%d: %s",  __FILE__,         \
			       __LINE__, ((ecode)->reason));                   \
			ub_log_close();                                        \
			ub_sysfail();                                          \
		}                                                              \
	} while (0)

#define UB_RAISE(ecode) UB_ENSURE(0, ecode)

#endif /* UB_EXCEPT_H */
