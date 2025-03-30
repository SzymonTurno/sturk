#include "message.h"
#include "ub/os/mem.h"

#define MSG_SIZE sizeof(struct Message)

static struct UBchan* chan_create(const char* topic, const struct UBloadVt* vp)
{
	struct UBchan* self = ub_malloc(sizeof(*self));
	struct Chan* c = ub_dict_data(self);

	ub_dict_setkey(self, strcpy(ub_malloc(strlen(topic) + 1), topic));
	c->vp = vp;
	c->list = NULL;
	c->mutex = ub_mutex_create(UB_MUTEX_PROTOCOL_PRIO_INHERIT);
	c->offset = (vp->size() / MSG_SIZE + 1) * MSG_SIZE;
	c->pool = ub_pool_create(c->offset + MSG_SIZE);
	return self;
}

static void chan_destroy(struct UBchan* chan)
{
	struct Chan* c = ub_dict_data(chan);

	if (c->pool)
		ub_pool_destroy(c->pool);
	c->pool = NULL;
	ub_mutex_destroy(c->mutex);
	c->mutex = NULL;
	while (c->list)
		ub_free(ub_list_rem(&c->list));
	c->vp = NULL;
	ub_free(ub_dict_getkey(chan));
	ub_dict_setkey(chan, NULL);
	ub_free(chan);
}

static void dict_destroy(struct UBchan* dict)
{
	for (struct UBrbnode *i = NULL, *p = NULL;;) {
		i = ub_rb_deepest(ub_dict_cast(dict));
		p = ub_rb_parent(i);
		chan_destroy(ub_dict_container(i, UBchan));
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

	*ub_list_data(self) = chan;
	return self;
}

static struct ScriberList* slist_create(struct UBscriber* scriber)
{
	struct ScriberList* self = ub_malloc(sizeof(*self));

	*ub_list_data(self) = scriber;
	return self;
}

static void ins_msg(struct UBscriber* scriber, struct Message* msg)
{
	struct Qentry* entry = ub_pool_alloc(scriber->pool);

	*ub_cirq_data(entry) = msg;
	ub_waitq_ins(scriber->q, ub_cirq_cast(entry));
}

static void notify(struct Chan* chan, struct Message* msg)
{
	int n = 0;

	ub_mutex_lock(chan->mutex);
	msg_lock(msg);
	for (UB_LIST_ITER(i, &chan->list)) {
		ins_msg(*ub_list_data(*i), msg);
		++n;
	}
	msg_unlock(msg, n);
	ub_mutex_unlock(chan->mutex);
}

static void unscribe(struct UBchan* chan, UBscriber* scriber)
{
	struct Chan* c = ub_dict_data(chan);

	ub_mutex_lock(c->mutex);
	for (UB_LIST_ITER(i, &c->list))
		if (*ub_list_data(*i) == scriber) {
			ub_free(ub_list_rem(i));
			break;
		}
	ub_mutex_unlock(c->mutex);
}

UBroker* ub_broker_create(const struct UBloadVt* vp)
{
	struct UBroker* self = ub_malloc(sizeof(*self));

	self->vp = vp;
	self->dict = NULL;
	self->list = NULL;
	self->mutex = ub_mutex_create(UB_MUTEX_PROTOCOL_PRIO_INHERIT);
	return self;
}

void ub_broker_destroy(UBroker* broker)
{
	ub_mutex_destroy(broker->mutex);
	broker->mutex = NULL;
	while (broker->list)
		ub_scriber_destroy(*ub_list_data(ub_list_rem(&broker->list)));

	if (broker->dict)
		dict_destroy(broker->dict);
	broker->dict = NULL;
	broker->vp = NULL;
	ub_free(broker);
}

UBchan* ub_broker_search(UBroker* broker, const char* topic)
{
	struct UBchan* c = NULL;

	ub_mutex_lock(broker->mutex);
	c = ub_dict_find(broker->dict, topic);
	if (!c) {
		c = chan_create(topic, broker->vp);
		broker->dict = ub_dict_ins(broker->dict, c);
	}
	ub_mutex_unlock(broker->mutex);
	return c;
}

UBscriber* ub_scriber_create(UBroker* broker)
{
	UBscriber* self = ub_malloc(sizeof(*self));

	self->broker = broker;
	ub_mutex_lock(broker->mutex);
	broker->list = ub_list_ins(broker->list, slist_create(self));
	ub_mutex_unlock(broker->mutex);
	self->list = NULL;
	self->msg = NULL;
	self->pool = ub_pool_create(sizeof(struct Qentry));
	self->q = ub_waitq_create();
	return self;
}

void ub_scriber_destroy(UBscriber* scriber)
{
	ub_scriber_release(scriber);
	ub_waitq_destroy(scriber->q);
	scriber->q = NULL;
	ub_pool_destroy(scriber->pool);
	scriber->pool = NULL;
	while (scriber->list) {
		unscribe(*ub_list_data(scriber->list), scriber);
		ub_free(ub_list_rem(&scriber->list));
	}
	ub_mutex_lock(scriber->broker->mutex);
	for (UB_LIST_ITER(i, &scriber->broker->list))
		if (*ub_list_data(*i) == scriber) {
			ub_free(ub_list_rem(i));
			break;
		}
	ub_mutex_unlock(scriber->broker->mutex);
	scriber->broker = NULL;
	ub_free(scriber);
}

UBload* ub_scriber_await(UBscriber* scriber, UBchan** chan)
{
	UBload* ret = NULL;
	struct UBcirq* q = NULL;
	struct Qentry* qe = NULL;

	ub_scriber_release(scriber);
	q = ub_waitq_rem(scriber->q);
	if (q) {
		qe = ub_cirq_container(q, struct Qentry);
		scriber->msg = *ub_cirq_data(qe);
		ub_pool_free(scriber->pool, qe);
		if (chan)
			*chan = scriber->msg->chan;
		ret = msg_getload(scriber->msg);
	}
	return ret;
}

void ub_scriber_release(UBscriber* scriber)
{
	if (scriber->msg)
		msg_release(scriber->msg);
	scriber->msg = NULL;
}

const char* ub_get_topic(UBchan* chan)
{
	return ub_dict_getkey(chan);
}

void ub_lish(UBchan* chan, ...)
{
	struct Message* msg = NULL;
	va_list args;

	va_start(args, chan);
	msg = msg_create(chan, args);
	va_end(args);
	notify(ub_dict_data(chan), msg);
}

void ub_scribe(UBscriber* scriber, const char* topic)
{
	struct UBchan* chan = ub_broker_search(scriber->broker, topic);
	struct Chan* c = ub_dict_data(chan);

	scriber->list = ub_list_ins(scriber->list, clist_create(chan));
	ub_mutex_lock(c->mutex);
	c->list = ub_list_ins(c->list, slist_create(scriber));
	ub_mutex_unlock(c->mutex);
}
