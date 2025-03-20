#ifndef DEFS_H
#define DEFS_H

#include "ub/mom/lish.h"
#include "ub/pool.h"
#include "ub/list.h"
#include "ub/cirq.h"
#include "ub/os/mutex.h"
#include "ub/os/sem.h"

struct UBloader {
	const struct UBloadVt* vp;
	UBpool* pool;
};

UB_LIST(struct UBeaterList, void*);

struct UBchan {
	struct UBloader* loader;
	struct UBeaterList* list;
};

UB_LIST(struct ChanList, struct UBchan*);

struct UBscriber {
	void* eater;
	struct ChanList* list;
};

UB_CIRQ(struct MessQ, struct UBmess);

struct UBus {
	UBpool* pool;
	UBmutex* mut;
	UBsem* sem;
	struct MessQ* q;
};

#endif /* DEFS_H */
