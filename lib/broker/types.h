#ifndef TYPES_H
#define TYPES_H

#include "UB/broker.h"
#include "UB/pool.h"
#include "UB/list.h"
#include "UB/dict.h"
#include "UB/cirq.h"
#include "UB/waitq.h"
#include "UB/os/mutex.h"

LIST(struct ScriberList, struct UBscriber*);
struct Chan {
	const struct UBloadVt* vp;
	struct ScriberList* list;
	UBmutex* mutex;
	UBpool* pool;
	size_t offset;
};

DICT(struct UBchan, struct Chan);
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

LIST(struct ChanList, struct UBchan*);
CIRQ(struct Qentry, struct Message*);
struct UBscriber {
	struct UBroker* broker;
	struct ChanList* list;
	struct Message* msg;
	UBpool* pool;
	UBwaitQ* q;
};

#endif /* TYPES_H */
