#ifndef CN_STR_H
#define CN_STR_H

#include "cn/list.h"
#include "cn/cirq.h"
#include "cn/dict.h"

CN_LIST(struct CnStrlist, char*);

CN_CIRQ(struct CnStrq, char*);

CN_DICT(struct CnStrbag, union { int n; void* align; });

char* cn_newstr(const char* str);

struct CnStrlist* cn_strlist_ins(struct CnStrlist* list, char* str);

char* cn_strlist_rem(struct CnStrlist** listp);

struct CnStrq* cn_strq_ins(struct CnStrq* q, char* str);

char* cn_strq_rem(struct CnStrq** qp);

struct CnStrbag* cn_strbag_ins(struct CnStrbag* bag, const char* str);

struct CnStrbag* cn_strbag_rem(struct CnStrbag* bag, const char* str);

int cn_strbag_count(struct CnStrbag* bag);

void cn_strbag_destroy(struct CnStrbag* bag);

#endif /* CN_STR_H */
