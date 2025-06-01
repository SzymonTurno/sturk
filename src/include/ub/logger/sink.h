#ifndef CN_LOGGER_SINK_H
#define CN_LOGGER_SINK_H

#include "ub/os/fstream.h"

typedef struct CnLogsink CnLogsink;

CnLogsink* cn_logsink_create(void);

void cn_logsink_push(CnLogsink* sink, UBfstream* stream);

void cn_logsink_pop(CnLogsink* sink, UBfstream* stream);

void cn_logsink_vprint(CnLogsink* sink, const char* format, va_list vlist);

void cn_logsink_print(CnLogsink* sink, const char* format, ...);

void cn_logsink_destroy(CnLogsink* sink);

#endif /* CN_LOGGER_SINK_H */
