/**
 * @file cn/logger/except.h
 *
 * @brief Exceptions.
 */

#ifndef CN_LOGGER_EXCEPT_H
#define CN_LOGGER_EXCEPT_H

#include "cn/logger/trace.h"
#include "cn/os/sys.h"

#ifndef CN_EXCEPTIONS_EN

/**
 * @def CN_EXCEPTIONS_EN
 *
 * @brief Exceptions enabled.
 */
#define CN_EXCEPTIONS_EN 0

#endif /* CN_EXCEPTIONS_EN */

/**
 * @def CN_ENSURE(cond, lvl, e)
 *
 * @brief Raise an exception if the condition is not met.
 *
 * @param[in] cond The condition.
 * @param[in] lvl The exception level: WARNING or ERROR.
 * @param[in] e The exception.
 */
#define CN_ENSURE(cond, lvl, e)                                                \
	do {                                                                   \
		enum CnTraceLvl _lvl = (lvl);                                  \
                                                                               \
		if (CN_EXCEPTIONS_EN && !(cond)) {                             \
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

/**
 * @def CN_RAISE(lvl, e)
 *
 * @brief Raise an exception.
 *
 * @param[in] lvl The exception level (WARNING or ERROR).
 * @param[in] e The exception.
 */
#define CN_RAISE(lvl, e) CN_ENSURE(0, lvl, e)

/**
 * @def CN_ENSURE_MEMORY(lvl, ptr)
 *
 * @brief Raise an exception and return NULL if the pointer is NULL.
 *
 * @param[in] ptr The pointer.
 * @param[in] lvl Exception level.
 */
#define CN_ENSURE_MEMORY(ptr, lvl)                                             \
	do {                                                                   \
		if ((ptr) == NULL) {                                           \
			RAISE(lvl, null_param);                                \
			return NULL;                                           \
		}                                                              \
	} while (0)

/**
 * @struct CnException
 *
 * @brief Exception.
 */
struct CnException {
	/**
	 * @var const char* reason
	 *
	 * @brief The reason that caused the exception.
	 */
	const char* reason;
};

/** Exception: "null_param". */
static const struct CnException cn_except_null_param /**/
	= {"Null param."};

/** Exception: "alloc_fail". */
static const struct CnException cn_except_alloc_fail /**/
	= {"Memory allocation failed."};

/** Exception: "sem_fail". */
static const struct CnException cn_except_sem_fail /**/
	= {"Semaphore failure."};

/** Exception: "mutex_fail". */
static const struct CnException cn_except_mutex_fail /**/
	= {"Mutex failure."};

/** Exception: "not_supported". */
static const struct CnException cn_except_not_supported /**/
	= {"Not supported."};

/** Exception: "sanity_fail". */
static const struct CnException cn_except_sanity_fail /**/
	= {"Sanity check failed."};

#endif /* CN_LOGGER_EXCEPT_H */
