#ifndef UB_MOM_LISH_H
#define UB_MOM_LISH_H

#include "ub/mom/chan.h"

struct UBmess {
	UBchan* chan;
	UBload load;
};

typedef struct UBus UBus;

struct UBmess ub_mess_create(UBchan* chan, ...);

void ub_lish(struct UBmess msg);

UBus* ub_bus_create(void);

void ub_bus_destroy(UBus* bus);

void ub_bus_ins(UBus* bus, struct UBmess msg);

struct UBmess ub_bus_rem(UBus* bus);

#endif /* UB_MOM_LISH_H */
