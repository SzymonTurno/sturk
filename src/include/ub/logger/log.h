#ifndef UB_LOG_H
#define UB_LOG_H

#include "ub/os/fstream.h"

enum UBlogLvl {
	UB_UNKNOWN = 0,
	UB_DEBUG,
	UB_INFO,
	UB_WARNING,
	UB_ERROR,
	UB_N_LOG_LVLS
};

void ub_log_attach(enum UBlogLvl lvl, UBfstream* stream);

void ub_log_detach(enum UBlogLvl lvl, UBfstream* stream);

void ub_log(enum UBlogLvl lvl, const char* tag, const char* format, ...);

void ub_log_cleanup(void);

#define UB_LOG(lvl, tag, ...)                                                 \
	do {                                                                   \
		if (UB_LOG_EN)                                                 \
			ub_log(lvl, tag, __VA_ARGS__);                         \
	} while (0)

#endif /* UB_LOG_H */
