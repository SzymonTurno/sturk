#ifndef CN_LOGGER_EXCEPT_H
#define CN_LOGGER_EXCEPT_H

#include "cn/logger/log.h"
#include "cn/os/sys.h"

struct CnException {
	const char* reason;
};

static const struct {
	struct CnException null_param[1];
	struct CnException alloc_fail[1];
	struct CnException sem_fail[1];
	struct CnException mutex_fail[1];
	struct CnException not_supported[1];
} CN_ECODES = {
	.null_param = {{ "Null param." }},
	.alloc_fail = {{ "Memory allocation failed." }},
	.sem_fail = {{ "Semaphore failure." }},
	.mutex_fail = {{ "Mutex failure." }},
	.not_supported = {{ "Not supported." }}
};

#ifdef CN_EXCEPTIONS_EN

#define CN_ENSURE(cond, ecode)                                                \
	do {                                                                   \
		if (CN_EXCEPTIONS_EN && !cond) {                               \
			CN_LOG(CN_ERROR, NULL, "%s:%d: %s",  __FILE__,         \
				__LINE__, ((ecode)->reason));                  \
			cn_log_cleanup();                                      \
			cn_sysfail();                                          \
		}                                                              \
	} while (0)

#else /* CN_EXCEPTIONS_EN */

#define CN_ENSURE(cond, ecode)

#endif /* CN_EXCEPTIONS_EN */

#define CN_RAISE(ecode) CN_ENSURE(0, ecode)

#endif /* CN_LOGGER_EXCEPT_H */
