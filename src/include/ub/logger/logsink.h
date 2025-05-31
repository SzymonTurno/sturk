#ifndef CY_LOGSINK_H
#define CY_LOGSINK_H

#include "ub/os/fstream.h"

typedef struct CyLogsink CyLogsink;

CyLogsink* cy_logsink_create(void);

void cy_logsink_push(CyLogsink* sink, UBfstream* stream);

void cy_logsink_pop(CyLogsink* sink, UBfstream* stream);

void cy_logsink_vprint(CyLogsink* sink, const char* format, va_list vlist);

void cy_logsink_print(CyLogsink* sink, const char* format, ...);

void cy_logsink_destroy(CyLogsink* sink);

#endif /* CY_LOGSINK_H */
