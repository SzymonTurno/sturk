#ifndef UB_MOM_SCRIBE_H
#define UB_MOM_SCRIBE_H

#include "ub/mom/chan.h"

typedef struct UBscriber UBscriber;

UBscriber* ub_scriber_create(void* eater, size_t size);

void ub_scriber_destroy(UBscriber* scriber);

void ub_scribe(UBscriber* scriber, UBchan* chan, UBeatPrio prio);

#endif /* UB_MOM_SCRIBE_H */
