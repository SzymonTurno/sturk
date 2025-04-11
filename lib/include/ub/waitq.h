#ifndef UB_WAITQ_H
#define UB_WAITQ_H

#include "ub/cirq.h"

typedef struct UBwaitQ UBwaitQ;

UBwaitQ* ub_waitq_create(void);

void ub_waitq_destroy(UBwaitQ* waitq);

void ub_waitq_ins(UBwaitQ* waitq, struct UBinode* entry);

struct UBinode* ub_waitq_rem(UBwaitQ* waitq);

struct UBinode* ub_waitq_tryrem(UBwaitQ* waitq);

#endif /* UB_WAITQ_H */
