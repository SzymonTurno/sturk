#ifndef UB_LOGGER_H
#define UB_LOGGER_H

#ifndef UB_DEBUG_EN
#define UB_DEBUG_EN 0
#endif /* UB_DEBUG_EN */

#ifndef UB_INFO_EN
#define UB_INFO_EN 0
#endif /* UB_DEBUG_EN */

#ifndef UB_WARNING_EN
#define UB_WARNING_EN 0
#endif /* UB_DEBUG_EN */

#ifndef UB_ERROR_EN
#define UB_ERROR_EN 0
#endif /* UB_DEBUG_EN */

#include <stddef.h>

enum UBlogLvl {
	UB_DEBUG = 0,
	UB_INFO,
	UB_WARNING,
	UB_ERROR
};

void ub_log(enum UBlogLvl lvl, const char* tag, const char* format, ...);

void ub_log_open(const char* filename);

void ub_log_close(void);

#define ub_log_error(text)                                                    \
	ub_log(UB_ERROR, NULL, "%s:%d: %s",  __FILE__, __LINE__, (text))

#define UB_LOG(lvl, tag, format, ...)                                         \
	do {                                                                   \
		if (lvl##_EN)                                                  \
			ub_log(lvl, tag, format, __VA_ARGS__);                 \
	} while (0)

#define UB_LOG_ERROR(text)                                                    \
	UB_LOG(UB_ERROR, NULL, "%s:%d: %s", __FILE__, __LINE__, (text))

#define UB_ENSURE(cond, text)                                                 \
	do {                                                                   \
		if (!cond)                                                     \
			UB_LOG_ERROR(text);                                    \
	} while (0)

#endif /* UB_LOGGER_H */
