#ifndef CY_STREAMBAG_H
#define CY_STREAMBAG_H

#include "ub/os/sysio.h"

typedef struct CyStreambag CyStreambag;

CyStreambag* cy_streambag_create(void);

void cy_streambag_push(CyStreambag* bag, UBfstream* stream);

void cy_streambag_pop(CyStreambag* bag, UBfstream* stream);

void cy_streambag_vprint(CyStreambag* bag, const char* format, va_list vlist);

void cy_streambag_print(CyStreambag* bag, const char* format, ...);

void cy_streambag_destroy(CyStreambag* bag);

#endif /* CY_STREAMBAG_H */
