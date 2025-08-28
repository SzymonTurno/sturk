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

#include "cantil/arith.h"
#include "cantil/graph.h"
#include "cantil/logger/except.h"
#include "cantil/logger/trace.h"
#include "cantil/os/mem.h"
#include "cantil/rbtree.h"
#include "cantil/str.h"
#include "message.h"

#define MSG_SIZE sizeof(struct Message)

static CnChannel* channel_create(CnBroker* broker, const char* topic)
{
	CnChannel* self = NEW(CnChannel);
	struct ChannelData* data = dict_datap(self);

	dict_setk(self, newstr(topic));
	data->broker = broker;
	data->mutex = mutex_create(MUTEX_POLICY_PRIO_INHERIT);
	data->list = NULL;
	return self;
}

static void channel_destroy(CnChannel* ch)
{
	struct ChannelData* data = dict_datap(ch);

	ENSURE(!data->list, ERROR, sanity_fail);
	mutex_destroy(data->mutex);
	data->mutex = NULL;
	data->broker = NULL;
	cn_free(dict_getk(ch));
	dict_setk(ch, NULL);
	cn_free(ch);
}

static void dict_destroy(CnChannel* dict)
{
	for (struct CnRbnode *i = NULL, *p = NULL;;) {
		i = rb_first(&dict_cast(dict)->node, BST_POSTORDER);
		p = rb_parent(i);
		channel_destroy(
			container_of(dictnode_from(i), CnChannel, dictnode));
		if (!p)
			break;

		if (i == rb_left(p))
			graph_2vx(p)->nbor[BST_LEFT] = NULL;
		else
			graph_2vx(p)->nbor[BST_RIGHT] = NULL;
	}
}

static struct ChannelList* clist_create(CnChannel* ch)
{
	struct ChannelList* self = NEW(struct ChannelList);

	*graph_datap(self) = ch;
	return self;
}

static struct SubscriberList* slist_create(struct CnSubscriber* sber)
{
	struct SubscriberList* self = NEW(struct SubscriberList);

	*graph_datap(self) = sber;
	return self;
}

static void ins_msg(CnSubscriber* sber, struct Message* msg)
{
	struct Qentry* entry = pool_alloc(sber->broker->sbers.pool);

	if (!entry) {
		/* LCOV_EXCL_START */
		RAISE(ERROR, alloc_fail);
		return;
		/* LCOV_EXCL_STOP */
	}
	*graph_datap(entry) = msg;
	waitq_ins(sber->q, graph_2vx(entry));
}

static void notify(struct ChannelData* data, struct Message* msg)
{
	int n = 0;

	mutex_lock(data->mutex);
	msg_lock(msg);
	list_foreach (struct SubscriberList, i, &data->list) {
		ins_msg(*graph_datap(*i), msg);
		++n;
	}
	msg_unlock(msg, n);
	mutex_unlock(data->mutex);
}

static void unsubscribe(CnChannel* ch, CnSubscriber* sber)
{
	struct ChannelData* data = dict_datap(ch);

	mutex_lock(data->mutex);
	list_foreach (struct SubscriberList, i, &data->list)
		if (*graph_datap(*i) == sber) {
			cn_free(list_rem(i));
			break;
		}
	mutex_unlock(data->mutex);
}

static CnLoad* load_init(CnSubscriber* sber, struct Vertegs* node)
{
	struct Qentry* q = NULL;

	subscriber_unload(sber);
	if (!node)
		return NULL;
	q = graph_4vx(node, q);
	sber->msg = *graph_datap(q);
	pool_free(sber->broker->sbers.pool, q);
	return msg_2load(sber->msg);
}

CnBroker* cn_broker_create(const struct CnLoadVt* vp)
{
	struct CnBroker* self = NULL;

	ENSURE_MEM(vp, ERROR);
	ENSURE_MEM(vp->size, ERROR);
	ENSURE_MEM(vp->ctor, ERROR);
	ENSURE_MEM(vp->dtor, ERROR);
	self = NEW(struct CnBroker);
	self->vp = vp;
	self->mutex = mutex_create(MUTEX_POLICY_PRIO_INHERIT);
	self->channels.pool =
		pool_create((vp->size() / MSG_SIZE + 2) * MSG_SIZE);
	self->channels.dict = NULL;
	self->sbers.pool = pool_create(sizeof(struct Qentry));
	self->sbers.list = NULL;
	return self;
}

void cn_broker_destroy(CnBroker* broker)
{
	if (!broker)
		return;

	while (broker->sbers.list)
		subscriber_destroy(*graph_datap(broker->sbers.list));
	pool_destroy(broker->sbers.pool);
	broker->sbers.pool = NULL;
	msg_purge(broker);
	if (broker->channels.dict)
		dict_destroy(broker->channels.dict);
	broker->channels.dict = NULL;
	pool_destroy(broker->channels.pool);
	broker->channels.pool = NULL;
	mutex_destroy(broker->mutex);
	broker->mutex = NULL;
	broker->vp = NULL;
	cn_free(broker);
}

CnChannel* cn_broker_search(CnBroker* broker, const char* topic)
{
	struct CnChannel* ch = NULL;

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

CnSubscriber* cn_subscriber_create(CnBroker* broker)
{
	CnSubscriber* self = NULL;

	ENSURE_MEM(broker, ERROR);
	self = NEW(CnSubscriber);
	self->broker = broker;
	self->q = waitq_create();
	self->msg = NULL;
	self->list = NULL;
	mutex_lock(broker->mutex);
	broker->sbers.list = list_ins(broker->sbers.list, slist_create(self));
	mutex_unlock(broker->mutex);
	return self;
}

void cn_subscriber_destroy(CnSubscriber* sber)
{
	if (!sber)
		return;

	while (sber->list) {
		unsubscribe(*graph_datap(sber->list), sber);
		cn_free(list_rem(&sber->list));
	}

	while ((load_init(sber, waitq_tryrem(sber->q))))
		;
	waitq_destroy(sber->q);
	sber->q = NULL;
	mutex_lock(sber->broker->mutex);
	list_foreach (struct SubscriberList, i, &sber->broker->sbers.list)
		if (*graph_datap(*i) == sber) {
			cn_free(list_rem(i));
			break;
		}
	mutex_unlock(sber->broker->mutex);
	sber->broker = NULL;
	cn_free(sber);
}

CnLoad* cn_subscriber_await(CnSubscriber* sber)
{
	ENSURE_MEM(sber, ERROR);
	return load_init(sber, waitq_rem(sber->q));
}

CnLoad* cn_subscriber_poll(CnSubscriber* sber)
{
	ENSURE_MEM(sber, ERROR);
	return load_init(sber, waitq_tryrem(sber->q));
}

void cn_subscriber_unload(CnSubscriber* sber)
{
	ENSURE(sber, WARNING, null_param);
	if (!sber)
		return;

	if (sber->msg)
		msg_release(sber->msg);
	sber->msg = NULL;
}

const char* cn_channel_gettopic(const CnChannel* ch)
{
	return ch ? dict_getk(ch) : NULL;
}

void cn_publish(CnChannel* ch, ...)
{
	struct Message* msg = NULL;
	va_list args;

	va_start(args, ch);
	msg = msg_create(dict_datap(ch)->broker, args);
	va_end(args);
	msg->channel = ch;
	notify(dict_datap(ch), msg);
}

void cn_subscribe(CnSubscriber* sber, const char* topic)
{
	struct CnChannel* ch = NULL;
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

CnChannel* cn_load_getchan(const CnLoad* load)
{
	return load ? msg_4load(load)->channel : NULL;
}
