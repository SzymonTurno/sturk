#ifndef TYPES_H
#define TYPES_H

#include "ub/broker.h"
#include "ub/pool.h"
#include "ub/list.h"
#include "ub/dict.h"
#include "ub/waitq.h"
#include "ub/os/mutex.h"

UB_LIST(struct ScriberList, struct UBscriber*);
struct Chan {
	const struct UBloadVt* vp;
	struct ScriberList* list;
	UBmutex* mutex;
	UBpool* pool;
	size_t offset;
};

UB_DICT(struct UBchan, struct Chan);
struct UBroker {
	const struct UBloadVt* vp;
	struct UBchan* dict;
	struct ScriberList* list;
	UBmutex* mutex;
};

struct Message {
	struct UBchan* chan;
	UBmutex* mutex;
	union {
		int n_pending;
		void* align;
	} u;
};

UB_LIST(struct ChanList, struct UBchan*);
UB_CIRQ(struct Qentry, struct Message*);
struct UBscriber {
	struct UBroker* broker;
	struct ChanList* list;
	struct Message* msg;
	UBpool* pool;
	UBwaitQ* q;
};

#endif /* TYPES_H */
