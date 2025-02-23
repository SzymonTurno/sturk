#ifndef UB_MOM_CHAN_H
#define UB_MOM_CHAN_H

#include "ub/mom/loader.h"

typedef struct UBchan UBchan;

typedef unsigned UBeatPrio;

UBchan* ub_chan_create(UBloader* loader);

void ub_chan_destroy(UBchan* chan);

void ub_chan_attach(UBchan* chan, void* eater, UBeatPrio prio);

void ub_chan_detach(UBchan* chan, void* eater);

void ub_chan_upload(UBchan* chan, UBload load);

#endif /* UB_MOM_CHAN_H */
