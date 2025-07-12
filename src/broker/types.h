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

CIRQ(struct Qentry, struct Message*);

struct ChannelData {
	CnBroker* broker;
	struct SubscriberList* list;
	CnMutex* mutex;
};

DICT(struct CnChannel, struct ChannelData);

struct CnBroker {
	const struct CnLoadVt* vp;
	CnMutex* mutex;
	struct SubscriberList* list;
	struct {
		CnChannel* dict;
		CnPool* pool;
		size_t offset;
	} channels;
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

struct CnSubscriber {
	CnBroker* broker;
	CnWaitq* q;
	struct Message* msg;
	struct ChannelList* list;
};

#endif /* TYPES_H */
