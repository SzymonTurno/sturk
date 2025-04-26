#ifndef UB_LOGGER_H
#define UB_LOGGER_H

#include "ub/os/sysio.h"

typedef struct UBlogger UBlogger;

UBlogger* ub_logger_create(void);

void ub_logger_attach(UBlogger* logger, UBfstream* stream);

void ub_logger_detach(UBlogger* logger, UBfstream* stream);

void ub_logger_vprint(UBlogger* logger, const char* format, va_list vlist);

void ub_logger_print(UBlogger* logger, const char* format, ...);

void ub_logger_destroy(UBlogger* logger);

#endif /* UB_LOGGER_H */
