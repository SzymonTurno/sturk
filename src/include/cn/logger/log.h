#ifndef CN_LOGGER_LOG_H
#define CN_LOGGER_LOG_H

#include "cn/os/fstream.h"

enum CnLogLvl {
	CN_UNKNOWN = 0,
	CN_DEBUG,
	CN_INFO,
	CN_WARNING,
	CN_ERROR,
	CN_N_LOG_LVLS
};

void cn_log_attach(enum CnLogLvl lvl, CnFstream* stream);

void cn_log_detach(enum CnLogLvl lvl, CnFstream* stream);

void cn_log(enum CnLogLvl lvl, const char* tag, const char* format, ...);

void cn_log_cleanup(void);

#define CN_LOG(lvl, tag, ...)                                                 \
	do {                                                                   \
		if (CN_LOG_EN)                                                 \
			cn_log(lvl, tag, __VA_ARGS__);                         \
	} while (0)

#endif /* CN_LOGGER_LOG_H */
