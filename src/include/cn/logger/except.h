#ifndef CN_LOGGER_EXCEPT_H
#define CN_LOGGER_EXCEPT_H

#include "cn/logger/trace.h"
#include "cn/os/sys.h"

struct CnException {
	const char* reason;
};

static const struct {
	struct CnException NULL_PARAM[1];
	struct CnException ALLOC_FAIL[1];
	struct CnException SEM_FAIL[1];
	struct CnException MUTEX_FAIL[1];
	struct CnException NOT_SUPPORTED[1];
} CN_EXCEPT = {
	.NULL_PARAM = {{"Null param."}},
	.ALLOC_FAIL = {{"Memory allocation failed."}},
	.SEM_FAIL = {{"Semaphore failure."}},
	.MUTEX_FAIL = {{"Mutex failure."}},
	.NOT_SUPPORTED = {{"Not supported."}}};

#ifdef CN_EXCEPTIONS_EN

#define CN_ENSURE(cond, ecode)                                                 \
	do {                                                                   \
		if (CN_EXCEPTIONS_EN && !cond) {                               \
			CN_TRACE(                                              \
				CN_ERROR, NULL, "%s:%d: %s", __FILE__,         \
				__LINE__, ((ecode)->reason));                  \
			cn_logger_cleanup();                                   \
			cn_sysfail();                                          \
		}                                                              \
	} while (0)

#else /* CN_EXCEPTIONS_EN */

#define CN_ENSURE(cond, ecode)

#endif /* CN_EXCEPTIONS_EN */

#define CN_RAISE(ecode) CN_ENSURE(0, ecode)

#endif /* CN_LOGGER_EXCEPT_H */
