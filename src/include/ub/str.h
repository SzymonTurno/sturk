#ifndef CY_STR_H
#define CY_STR_H

#include "UB/list.h"
#include "UB/cirq.h"
#include "UB/dict.h"

LIST(struct CyStrlist, char*);

CIRQ(struct CyStrq, char*);

DICT(struct CyStrbag, union { int n; void* align; });

char* cy_newstr(const char* str);

struct CyStrlist* cy_strlist_ins(struct CyStrlist* list, char* str);

char* cy_strlist_rem(struct CyStrlist** listp);

struct CyStrq* cy_strq_ins(struct CyStrq* q, char* str);

char* cy_strq_rem(struct CyStrq** qp);

struct CyStrbag* cy_strbag_ins(struct CyStrbag* bag, const char* str);

struct CyStrbag* cy_strbag_rem(struct CyStrbag* bag, const char* str);

int cy_strbag_count(struct CyStrbag* bag);

void cy_strbag_destroy(struct CyStrbag* bag);

#endif /* CY_STR_H */
