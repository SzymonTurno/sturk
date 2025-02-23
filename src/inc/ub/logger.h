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

enum UBlogLvl {
	UB_DEBUG = 0,
	UB_INFO,
	UB_WARNING,
	UB_ERROR
};

void ub_log(enum UBlogLvl lvl, const char* tag, const char* format, ...);

void ub_log_initlock(void);

void ub_log_deinitlock(void);

void ub_log_inittest(const char* filename);

void ub_log_closetest(void);

#define ub_log_error(tag, format, ...)                                        \
	ub_log(UB_ERROR, tag, "%s:%d: "format, __FILE__, __LINE__, __VA_ARGS__)

#define UB_LOG(lvl, tag, format, ...)                                         \
	do {                                                                   \
		if (lvl##_EN)                                                  \
			ub_log(lvl, tag, format, __VA_ARGS__);                 \
	} while (0)

#define UB_LOG_ERROR(tag, format, ...)                                        \
	UB_LOG(UB_ERROR, tag, "%s:%d: "format, __FILE__, __LINE__, __VA_ARGS__)

#endif /* UB_LOGGER_H */
