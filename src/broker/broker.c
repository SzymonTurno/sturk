/*
BSD 3-Clause License

Copyright (c) 2025, Szymon Turno

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "sturk/broker.h"
#include "sturk/arith.h"
#include "sturk/cirq.h"
#include "sturk/dict.h"
#include "sturk/graph.h"
#include "sturk/io/except.h"
#include "sturk/io/logger.h"
#include "sturk/list.h"
#include "sturk/os/mem.h"
#include "sturk/os/mutex.h"
#include "sturk/pool.h"
#include "sturk/rbtree.h"
#include "sturk/str.h"
#include "sturk/waitq.h"

LIST(struct SubscriberList, StSubscriber*);

struct ChannelData {
	StBroker* broker;
	struct SubscriberList* list;
	StMutex* mutex;
};

DICT(struct StChannel, struct ChannelData);

struct StBroker {
	StMutex* mutex;
	struct {
		struct SubscriberList* list;
		StPool* pool;
	} sbers;
	struct {
		StChannel* dict;
		StPool* pool;
	} channels;
	size_t payload_size;
};

union Header {
	struct {
		union {
			size_t n_pending;
			/* The member "union StFreeList align" is overwritten
			 * when the message is returned to the pool. */
			union StFreeList align;
		} u;
		/* Mutex must be not the first member so that it is not
		 * overwritten when the message is returned to the pool. */
		StMutex* mutex;
		StChannel* channel;
		void (*cb)(struct StMessage);
	} s;
	StAlign align;
};

CIRQ(struct Qentry, union Header*);

LIST(struct ChannelList, StChannel*);

struct StSubscriber {
	StBroker* broker;
	StWaitQ* q;
	union Header* cached;
	struct ChannelList* list;
};

static StChannel* channel_create(StBroker* broker, const char* topic)
{
	StChannel* self = NEW(StChannel);
	struct ChannelData* data = dict_datap(self);

	dict_setk(self, newstr(topic));
	data->broker = broker;
	data->mutex = mutex_create(MUTEX_POLICY_PRIO_INHERIT);
	data->list = NULL;
	return self;
}

static void channel_destroy(StChannel* ch)
{
	struct ChannelData* data = dict_datap(ch);

	ENSURE(!data->list, ERROR, sanity_fail);
	mutex_destroy(data->mutex);
	data->mutex = NULL;
	data->broker = NULL;
	st_free(dict_getk(ch));
	dict_setk(ch, NULL);
	st_free(ch);
}

static void dict_destroy(StChannel* dict)
{
	for (struct StRbNode *i = NULL, *p = NULL;;) {
		i = rb_first(&dict_cast(dict)->node, BST_POSTORDER);
		p = rb_parent(i);
		channel_destroy(
			container_of(dictnode_from(i), StChannel, dictnode));
		if (!p)
			break;

		if (i == rb_left(p))
			graph_2vx(p)->nbor[BST_LEFT] = NULL;
		else
			graph_2vx(p)->nbor[BST_RIGHT] = NULL;
	}
}

static struct ChannelList* clist_create(StChannel* ch)
{
	struct ChannelList* self = NEW(struct ChannelList);

	*graph_datap(self) = ch;
	return self;
}

static struct SubscriberList* slist_create(struct StSubscriber* sber)
{
	struct SubscriberList* self = NEW(struct SubscriberList);

	*graph_datap(self) = sber;
	return self;
}

static void enqueue(StSubscriber* sber, union Header* h)
{
	struct Qentry* entry = pool_alloc(sber->broker->sbers.pool);

	if (!entry) {
		/* LCOV_EXCL_START */
		RAISE(ERROR, alloc_fail);
		return;
		/* LCOV_EXCL_STOP */
	}
	*graph_datap(entry) = h;
	waitq_ins(sber->q, graph_2vx(entry));
}

static void unsubscribe(StChannel* ch, StSubscriber* sber)
{
	struct ChannelData* data = dict_datap(ch);

	mutex_lock(data->mutex);
	for (struct SubscriberList** i = &data->list; *i; listit_next(&i))
		if (*graph_datap(*i) == sber) {
			st_free(list_rem(i));
			break;
		}
	mutex_unlock(data->mutex);
}

static void* initmsg(StSubscriber* sber, struct Vertegs* node)
{
	struct Qentry* q = NULL;

	ENSURE_MEM(sber, ERROR);
	subscriber_unload(sber);
	if (!node)
		return NULL;
	q = graph_4vx(node, q);
	sber->cached = *graph_datap(q);
	pool_free(sber->broker->sbers.pool, q);
	return &sber->cached[1];
}

static void recycle(StBroker* broker, union Header* h)
{
	void (*cb)(struct StMessage) = h->s.cb;

	if (cb)
		cb((struct StMessage){.payload = &h[1]});
	pool_free(broker->channels.pool, h);
}

static void purge(StPool* pool)
{
	union Header* h = NULL;

	while ((h = pool_tryalloc(pool))) {
		mutex_destroy(h->s.mutex);
		h->s.mutex = NULL;
		st_free(h);
	}
	pool_destroy(pool);
}

StBroker* st_broker_create(size_t payload_size)
{
	struct StBroker* self = NULL;

	self = NEW(struct StBroker);
	self->mutex = mutex_create(MUTEX_POLICY_PRIO_INHERIT);
	self->channels.pool = pool_create(0);
	self->channels.dict = NULL;
	self->sbers.pool = pool_create(sizeof(struct Qentry));
	self->sbers.list = NULL;
	self->payload_size = payload_size;
	return self;
}

void st_broker_destroy(StBroker* broker)
{
	if (!broker)
		return;

	while (broker->sbers.list)
		subscriber_destroy(*graph_datap(broker->sbers.list));
	pool_destroy(broker->sbers.pool);
	broker->sbers.pool = NULL;

	purge(broker->channels.pool);
	broker->channels.pool = NULL;
	if (broker->channels.dict)
		dict_destroy(broker->channels.dict);
	broker->channels.dict = NULL;

	mutex_destroy(broker->mutex);
	broker->mutex = NULL;
	st_free(broker);
}

StChannel* st_broker_search(StBroker* broker, const char* topic)
{
	struct StChannel* ch = NULL;

	ENSURE_MEM(broker, ERROR);
	mutex_lock(broker->mutex);
	ch = dict_find(broker->channels.dict, topic);
	if (!ch) {
		ch = channel_create(broker, topic);
		broker->channels.dict = dict_ins(broker->channels.dict, ch);
	}
	mutex_unlock(broker->mutex);
	return ch;
}

struct StMessage st_channel_allocmsg(StChannel* ch)
{
	struct ChannelData* chdata = NULL;
	union Header* h = NULL;

	ENSURE(ch, WARNING, null_param);
	if (!ch)
		return (struct StMessage){.payload = NULL};
	chdata = dict_datap(ch);
	h = pool_tryalloc(chdata->broker->channels.pool);
	if (!h) {
		h = st_alloc(
			sizeof(union Header) + chdata->broker->payload_size);
		h->s.mutex = mutex_create(MUTEX_POLICY_PRIO_INHERIT);
	}
	h->s.u.n_pending = 0;
	h->s.channel = ch;
	h->s.cb = NULL;
	return (struct StMessage){.payload = &h[1]};
}

void st_message_setcb(struct StMessage msg, void (*cb)(struct StMessage))
{
	union Header* h = NULL;

	ENSURE(msg.payload, ERROR, not_supported);
	h = (union Header*)msg.payload - 1;
	h->s.cb = cb;
}

StChannel* st_message_getchannel(struct StMessage msg)
{
	union Header* h = NULL;

	ENSURE_MEM(msg.payload, ERROR);
	h = (union Header*)msg.payload - 1;
	return h->s.channel;
}

void st_publish(struct StMessage* msg)
{
	union Header* h = (union Header*)msg->payload - 1;
	struct ChannelData* chdata = dict_datap(h->s.channel);
	size_t n = 0;

	msg->payload = NULL;
	mutex_lock(chdata->mutex);
	mutex_lock(h->s.mutex);
	for (struct SubscriberList** i = &chdata->list; *i; listit_next(&i)) {
		enqueue(*graph_datap(*i), h);
		++n;
	}
	h->s.u.n_pending = n;
	mutex_unlock(h->s.mutex);
	if (!n)
		recycle(chdata->broker, h);
	mutex_unlock(chdata->mutex);
}

StSubscriber* st_subscriber_create(StBroker* broker)
{
	StSubscriber* self = NULL;

	ENSURE_MEM(broker, ERROR);
	self = NEW(StSubscriber);
	self->broker = broker;
	self->q = waitq_create();
	self->cached = NULL;
	self->list = NULL;
	mutex_lock(broker->mutex);
	broker->sbers.list = list_ins(broker->sbers.list, slist_create(self));
	mutex_unlock(broker->mutex);
	return self;
}

void st_subscriber_destroy(StSubscriber* sber)
{
	if (!sber)
		return;

	while (sber->list) {
		unsubscribe(*graph_datap(sber->list), sber);
		st_free(list_rem(&sber->list));
	}

	while ((initmsg(sber, waitq_tryrem(sber->q))))
		;
	waitq_destroy(sber->q);
	sber->q = NULL;
	mutex_lock(sber->broker->mutex);
	for (struct SubscriberList** i = &sber->broker->sbers.list; *i;) {
		if (*graph_datap(*i) == sber) {
			st_free(list_rem(i));
			break;
		}
		listit_next(&i);
	}
	mutex_unlock(sber->broker->mutex);
	sber->broker = NULL;
	st_free(sber);
}

struct StMessage st_subscriber_await(StSubscriber* sber)
{
	return (struct StMessage){.payload = initmsg(sber, waitq_rem(sber->q))};
}

struct StMessage st_subscriber_poll(StSubscriber* sber)
{
	return (struct StMessage){
		.payload = initmsg(sber, waitq_tryrem(sber->q))};
}

void st_subscriber_unload(StSubscriber* sber)
{
	int last = 0;

	ENSURE(sber, WARNING, null_param);
	if (!sber)
		return;

	if (sber->cached) {
		mutex_lock(sber->cached->s.mutex);
		ENSURE(sber->cached->s.u.n_pending, ERROR, sanity_fail);
		last = (sber->cached->s.u.n_pending == 1);
		--sber->cached->s.u.n_pending;
		mutex_unlock(sber->cached->s.mutex);
	}

	if (last)
		recycle(sber->broker, sber->cached);
	sber->cached = NULL;
}

const char* st_channel_gettopic(const StChannel* ch)
{
	return ch ? dict_getk(ch) : NULL;
}

void st_subscribe(StSubscriber* sber, const char* topic)
{
	struct StChannel* ch = NULL;
	struct ChannelData* data = NULL;

	if (!sber->broker) {
		RAISE(WARNING, null_param);
		return;
	}
	ch = broker_search(sber->broker, topic);
	data = dict_datap(ch);
	sber->list = list_ins(sber->list, clist_create(ch));
	mutex_lock(data->mutex);
	data->list = list_ins(data->list, slist_create(sber));
	mutex_unlock(data->mutex);
}
