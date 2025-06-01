#ifndef CN_LOGGER_SINK_H
#define CN_LOGGER_SINK_H

#include "cn/os/fstream.h"

typedef struct CnLogsink CnLogsink;

CnLogsink* cn_logsink_create(void);

void cn_logsink_ins(CnLogsink* sink, CnFstream* stream);

void cn_logsink_rem(CnLogsink* sink, CnFstream* stream);

void cn_logsink_vprint(CnLogsink* sink, const char* format, va_list vlist);

void cn_logsink_print(CnLogsink* sink, const char* format, ...);

void cn_logsink_destroy(CnLogsink* sink);

#endif /* CN_LOGGER_SINK_H */
