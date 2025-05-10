#include "message.h"
#include "UB/arith.h"
#include "UB/rbtree.h"
#include "UB/debug/except.h"
#include "ub/os/mem.h"
#include <string.h>

#define MSG_SIZE sizeof(struct Message)

static struct UBstrnode* strnode_cont(struct UBrbnode* ptr)
{
	return container_of(ptr, struct UBstrnode, node);
}

static struct UBchan* chan_create(const char* topic, const struct UBloadVt* vp)
{
	struct UBchan* self = ub_malloc(sizeof(*self));
	struct Chan* c = dict_data(self);

	dict_setk(self, strcpy(ub_malloc(strlen(topic) + 1), topic));
	c->vp = vp;
	c->list = NULL;
	c->mutex = mutex_create(MUTEX_POLICY_PRIO_INHERIT);
	c->offset = (vp->size() / MSG_SIZE + 1) * MSG_SIZE;
	c->pool = pool_create(c->offset + MSG_SIZE);
	return self;
}

static void chan_destroy(struct UBchan* chan)
{
	struct Chan* c = dict_data(chan);

	msg_purge(chan);
	pool_destroy(c->pool);
	c->pool = NULL;
	mutex_destroy(c->mutex);
	c->mutex = NULL;
	while (c->list)
		ub_free(list_rem(&c->list));
	c->vp = NULL;
	ub_free(dict_getk(chan));
	dict_setk(chan, NULL);
	ub_free(chan);
}

static void dict_destroy(struct UBchan* dict)
{
	for (struct UBrbnode *i = NULL, *p = NULL;;) {
		i = rb_deepest(&dict_cast(dict)->node);
		p = rb_parent(i);
		chan_destroy(dict_cont(strnode_cont(i), UBchan));
		if (!p)
			break;

		if (i == p->left)
			p->left = NULL;
		else
			p->right = NULL;
	}
}

static struct ChanList* clist_create(struct UBchan* chan)
{
	struct ChanList* self = ub_malloc(sizeof(*self));

	*list_data(self) = chan;
	return self;
}

static struct ScriberList* slist_create(struct UBscriber* scriber)
{
	struct ScriberList* self = ub_malloc(sizeof(*self));

	*list_data(self) = scriber;
	return self;
}

static void ins_msg(struct UBscriber* scriber, struct Message* msg)
{
	struct Qentry* entry = pool_alloc(scriber->pool);

	*cirq_data(entry) = msg;
	waitq_ins(scriber->q, cirq_cast(entry));
}

static void notify(struct Chan* chan, struct Message* msg)
{
	int n = 0;

	mutex_lock(chan->mutex);
	msg_lock(msg);
	for (LIST_ITER(struct ScriberList, i, &chan->list)) {
		ins_msg(*list_data(*i), msg);
		++n;
	}
	msg_unlock(msg, n);
	mutex_unlock(chan->mutex);
}

static void unscribe(struct UBchan* chan, UBscriber* scriber)
{
	struct Chan* c = dict_data(chan);

	mutex_lock(c->mutex);
	for (LIST_ITER(struct ScriberList, i, &c->list))
		if (*list_data(*i) == scriber) {
			ub_free(list_rem(i));
			break;
		}
	mutex_unlock(c->mutex);
}

static UBload* load_init(UBscriber* scriber, struct UBinode* node)
{
	struct Qentry* q = NULL;

	scriber_release(scriber);
	if (!node)
		return NULL;
	q = cirq_cont(node, struct Qentry);
	scriber->msg = *cirq_data(q);
	pool_free(scriber->pool, q);
	return msg_getload(scriber->msg);
}

UBroker* ub_broker_create(const struct UBloadVt* vp)
{
	struct UBroker* self = NULL;

	ENSURE(vp && vp->size && vp->ctor && vp->dtor, ECODES.null_param);
	self = ub_malloc(sizeof(*self));
	self->vp = vp;
	self->dict = NULL;
	self->list = NULL;
	self->mutex = mutex_create(MUTEX_POLICY_PRIO_INHERIT);
	return self;
}

void ub_broker_destroy(UBroker* broker)
{
	if (!broker)
		return;

	while (broker->list)
		scriber_destroy(*list_data(broker->list));
	mutex_destroy(broker->mutex);
	broker->mutex = NULL;
	if (broker->dict)
		dict_destroy(broker->dict);
	broker->dict = NULL;
	broker->vp = NULL;
	ub_free(broker);
}

UBchan* ub_broker_search(UBroker* broker, const char* topic)
{
	struct UBchan* c = NULL;

	ENSURE(broker, ECODES.null_param);
	mutex_lock(broker->mutex);
	c = dict_find(broker->dict, topic);
	if (!c) {
		c = chan_create(topic, broker->vp);
		broker->dict = dict_ins(broker->dict, c);
	}
	mutex_unlock(broker->mutex);
	return c;
}

UBscriber* ub_scriber_create(UBroker* broker)
{
	UBscriber* self = NULL;

	ENSURE(broker, ECODES.null_param);
	self = ub_malloc(sizeof(*self));
	self->broker = broker;
	mutex_lock(broker->mutex);
	broker->list = list_ins(broker->list, slist_create(self));
	mutex_unlock(broker->mutex);
	self->pool = pool_create(sizeof(struct Qentry));
	self->q = waitq_create();
	self->msg = NULL;
	self->list = NULL;
	return self;
}

void ub_scriber_destroy(UBscriber* scriber)
{
	if (!scriber)
		return;

	while (scriber->list) {
		unscribe(*list_data(scriber->list), scriber);
		ub_free(list_rem(&scriber->list));
	}

	while ((load_init(scriber, waitq_tryrem(scriber->q))))
		;
	waitq_destroy(scriber->q);
	scriber->q = NULL;
	pool_destroy(scriber->pool);
	scriber->pool = NULL;
	mutex_lock(scriber->broker->mutex);
	for (LIST_ITER(struct ScriberList, i, &scriber->broker->list))
		if (*list_data(*i) == scriber) {
			ub_free(list_rem(i));
			break;
		}
	mutex_unlock(scriber->broker->mutex);
	scriber->broker = NULL;
	ub_free(scriber);
}

UBload* ub_scriber_await(UBscriber* scriber, UBchan** chan)
{
	UBload* ret = NULL;

	ENSURE(scriber, ECODES.null_param);
	ret = load_init(scriber, waitq_rem(scriber->q));
	if (chan)
		*chan = scriber->msg->chan;
	return ret;
}

UBload* ub_scriber_poll(UBscriber* scriber, UBchan** chan)
{
	UBload* ret = NULL;

	ENSURE(scriber, ECODES.null_param);
	ret = load_init(scriber, waitq_tryrem(scriber->q));
	if (chan)
		*chan = scriber->msg ? scriber->msg->chan : NULL;
	return ret;
}

void ub_scriber_release(UBscriber* scriber)
{
	ENSURE(scriber, ECODES.null_param);
	if (scriber->msg)
		msg_release(scriber->msg);
	scriber->msg = NULL;
}

const char* ub_get_topic(UBchan* chan)
{
	return dict_getk(chan);
}

void ub_lish(UBchan* chan, ...)
{
	struct Message* msg = NULL;
	va_list args;

	va_start(args, chan);
	msg = msg_create(chan, args);
	va_end(args);
	notify(dict_data(chan), msg);
}

void ub_scribe(UBscriber* scriber, const char* topic)
{
	struct UBchan* chan = NULL;
	struct Chan* c = NULL;

	ENSURE(scriber, ECODES.null_param);
	chan = broker_search(scriber->broker, topic);
	c = dict_data(chan);
	scriber->list = list_ins(scriber->list, clist_create(chan));
	mutex_lock(c->mutex);
	c->list = list_ins(c->list, slist_create(scriber));
	mutex_unlock(c->mutex);
}
