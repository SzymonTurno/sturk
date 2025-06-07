#ifndef TYPES_H
#define TYPES_H

#include "cantil/broker.h"
#include "cantil/cirq.h"
#include "cantil/dict.h"
#include "cantil/list.h"
#include "cantil/os/mutex.h"
#include "cantil/pool.h"
#include "cantil/waitq.h"

LIST(struct SubscriberList, CnSubscriber*);

struct ChannelData {
	const struct CnLoadVt* vp;
	struct SubscriberList* list;
	CnMutex* mutex;
	CnPool* pool;
	size_t offset;
};

DICT(struct CnChannel, struct ChannelData);

struct CnBroker {
	const struct CnLoadVt* vp;
	CnChannel* dict;
	struct SubscriberList* list;
	CnMutex* mutex;
};

struct Message {
	CnChannel* channel;
	CnMutex* mutex;
	union {
		int n_pending;
		void* align;
	} u;
};

LIST(struct ChannelList, CnChannel*);

CIRQ(struct Qentry, struct Message*);

struct CnSubscriber {
	CnBroker* broker;
	struct ChannelList* list;
	struct Message* msg;
	CnPool* pool;
	CnWaitq* q;
};

#endif /* TYPES_H */
