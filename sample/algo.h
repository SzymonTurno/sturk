#ifndef ALGO_H
#define ALGO_H

#include "UB/list.h"
#include "UB/cirq.h"

LIST(struct StrList, const char*);

CIRQ(struct StrQ, const char*);

struct StrList* strlist_ins(struct StrList* list, const char* str);

const char* strlist_rem(struct StrList** listp);

struct StrQ* strq_ins(struct StrQ* q, const char* str);

const char* strq_rem(struct StrQ** qp);

#endif /* ALGO_H */
