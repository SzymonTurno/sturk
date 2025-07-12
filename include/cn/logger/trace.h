#ifndef CN_LOGGER_TRACE_H
#define CN_LOGGER_TRACE_H

#include "cn/os/fstream.h"

#ifndef CN_LOGGER_EN

#define CN_LOGGER_EN 0

#endif /* CN_LOGGER_EN */

#define CN_TRACE(lvl, tag, ...)                                                \
	do {                                                                   \
		if (CN_LOGGER_EN)                                              \
			cn_trace(lvl, tag, __VA_ARGS__);                       \
	} while (0)

enum CnTraceLvl {
	CN_UNKNOWN = 0,
	CN_DEBUG,
	CN_INFO,
	CN_WARNING,
	CN_ERROR,
	CN_N_TRACE_LVLS
};

void cn_trace(enum CnTraceLvl lvl, const char* tag, const char* format, ...);

void cn_logger_attach(enum CnTraceLvl lvl, CnFstream* stream);

void cn_logger_detach(enum CnTraceLvl lvl, CnFstream* stream);

void cn_logger_cleanup(void);

#endif /* CN_LOGGER_TRACE_H */
