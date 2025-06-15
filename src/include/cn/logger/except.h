#ifndef CN_LOGGER_EXCEPT_H
#define CN_LOGGER_EXCEPT_H

#include "cn/logger/trace.h"
#include "cn/os/sys.h"

struct CnException {
	const char* reason;
};

static const struct CnException cn_except_null_param = {"Null param."};
static const struct CnException cn_except_alloc_fail = {
	"Memory allocation failed."};
static const struct CnException cn_except_sem_fail = {"Semaphore failure."};
static const struct CnException cn_except_mutex_fail = {"Mutex failure."};
static const struct CnException cn_except_not_supported = {"Not supported."};
static const struct CnException cn_except_sanity_fail = {"Sanity check failed."};

#ifdef CN_EXCEPTIONS_EN

#define CN_ENSURE(cond, lvl, e)                                                \
	do {                                                                   \
		enum CnTraceLvl _lvl = (lvl);                                  \
                                                                               \
		if (CN_EXCEPTIONS_EN && !cond) {                               \
			if (CN_ERROR == _lvl) {                                \
				CN_TRACE(                                      \
					CN_ERROR, NULL, "%s:%d: %s", __FILE__, \
					__LINE__, cn_except_##e.reason);       \
				cn_logger_cleanup();                           \
				cn_sysfail();                                  \
			} else if (CN_WARNING == _lvl) {                       \
				CN_TRACE(                                      \
					CN_WARNING, NULL, "%s:%d: %s",         \
					__FILE__, __LINE__,                    \
					cn_except_##e.reason);                 \
			} else {                                               \
				CN_TRACE(                                      \
					CN_ERROR, NULL, "%s:%d: %s", __FILE__, \
					__LINE__,                              \
					cn_except_not_supported.reason);       \
			}                                                      \
		}                                                              \
	} while (0)

#else /* CN_EXCEPTIONS_EN */

#define CN_ENSURE(cond, lvl, e)

#endif /* CN_EXCEPTIONS_EN */

#define CN_RAISE(lvl, e) CN_ENSURE(0, lvl, e)

#endif /* CN_LOGGER_EXCEPT_H */
