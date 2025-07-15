#include "cantil/arith.h"
#include "cantil/logger/except.h"
#include "cantil/logger/trace.h"
#include "cantil/rbtree.h"
#include "cantil/str.h"
#include "cn/os/mem.h"
#include "message.h"

#define MSG_SIZE sizeof(struct Message)

static CnChannel* channel_create(CnBroker* broker, const char* topic)
{
	CnChannel* self = cn_malloc(sizeof(*self));
	struct ChannelData* data = dict_data(self);

	dict_setk(self, newstr(topic));
	data->broker = broker;
	data->mutex = mutex_create(MUTEX_POLICY_PRIO_INHERIT);
	data->list = NULL;
	return self;
}

static void channel_destroy(CnChannel* ch)
{
	struct ChannelData* data = dict_data(ch);

	ENSURE(!data->list, ERROR, sanity_fail);
	msg_purge(data->broker);
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
		i = rb_deepest(&dict_cast(dict)->node);
		p = rb_parent(i);
		channel_destroy(
			container_of(strnode_from(i), CnChannel, strnode));
		if (!p)
			break;

		if (i == p->left)
			p->left = NULL;
		else
			p->right = NULL;
	}
}

static struct ChannelList* clist_create(CnChannel* ch)
{
	struct ChannelList* self = cn_malloc(sizeof(*self));

	*list_data(self) = ch;
	return self;
}

static struct SubscriberList* slist_create(struct CnSubscriber* sber)
{
	struct SubscriberList* self = cn_malloc(sizeof(*self));

	*list_data(self) = sber;
	return self;
}

static void ins_msg(CnSubscriber* sber, struct Message* msg)
{
	struct Qentry* entry = cn_malloc(sizeof(*entry));

	if (!entry) {
		/* LCOV_EXCL_START */
		RAISE(ERROR, alloc_fail);
		return;
		/* LCOV_EXCL_STOP */
	}
	*cirq_data(entry) = msg;
	waitq_ins(sber->q, cirq_cast(entry));
}

static void notify(struct ChannelData* data, struct Message* msg)
{
	int n = 0;

	mutex_lock(data->mutex);
	msg_lock(msg);
	list_iter (i, &data->list) {
		ins_msg(*list_data(*i), msg);
		++n;
	}
	msg_unlock(msg, n);
	mutex_unlock(data->mutex);
}

static void unsubscribe(CnChannel* ch, CnSubscriber* sber)
{
	struct ChannelData* data = dict_data(ch);

	mutex_lock(data->mutex);
	list_iter (i, &data->list)
		if (*list_data(*i) == sber) {
			cn_free(list_rem(i));
			break;
		}
	mutex_unlock(data->mutex);
}

static CnLoad* load_init(CnSubscriber* sber, struct CnBinode* node)
{
	struct Qentry* q = NULL;

	subscriber_release(sber);
	if (!node)
		return NULL;
	q = cirq_from(node, struct Qentry);
	sber->msg = *cirq_data(q);
	cn_free(q);
	return msg_getload(sber->msg);
}

CnBroker* cn_broker_create(const struct CnLoadVt* vp)
{
	struct CnBroker* self = NULL;

	ENSURE_MEMORY(ERROR, vp);
	ENSURE_MEMORY(ERROR, vp->size);
	ENSURE_MEMORY(ERROR, vp->ctor);
	ENSURE_MEMORY(ERROR, vp->dtor);
	self = cn_malloc(sizeof(*self));
	self->vp = vp;
	self->mutex = mutex_create(MUTEX_POLICY_PRIO_INHERIT);
	self->channels.offset = (vp->size() / MSG_SIZE + 1) * MSG_SIZE;
	self->channels.pool = pool_create(self->channels.offset + MSG_SIZE);
	self->channels.dict = NULL;
	self->list = NULL;
	return self;
}

void cn_broker_destroy(CnBroker* broker)
{
	if (!broker)
		return;

	while (broker->list)
		subscriber_destroy(*list_data(broker->list));
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

	ENSURE_MEMORY(ERROR, broker);
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

	ENSURE_MEMORY(ERROR, broker);
	self = cn_malloc(sizeof(*self));
	self->broker = broker;
	self->q = waitq_create();
	self->msg = NULL;
	self->list = NULL;
	mutex_lock(broker->mutex);
	broker->list = list_ins(broker->list, slist_create(self));
	mutex_unlock(broker->mutex);
	return self;
}

void cn_subscriber_destroy(CnSubscriber* sber)
{
	if (!sber)
		return;

	while (sber->list) {
		unsubscribe(*list_data(sber->list), sber);
		cn_free(list_rem(&sber->list));
	}

	while ((load_init(sber, waitq_tryrem(sber->q))))
		;
	waitq_destroy(sber->q);
	sber->q = NULL;
	mutex_lock(sber->broker->mutex);
	list_iter (i, &sber->broker->list)
		if (*list_data(*i) == sber) {
			cn_free(list_rem(i));
			break;
		}
	mutex_unlock(sber->broker->mutex);
	sber->broker = NULL;
	cn_free(sber);
}

CnLoad* cn_subscriber_await(CnSubscriber* sber, CnChannel** ch)
{
	CnLoad* ret = NULL;

	ENSURE_MEMORY(ERROR, sber);
	ret = load_init(sber, waitq_rem(sber->q));
	if (ch) {
		ENSURE(sber->msg, ERROR, null_param);
		*ch = sber->msg->channel;
	}
	return ret;
}

CnLoad* cn_subscriber_poll(CnSubscriber* sber, CnChannel** ch)
{
	CnLoad* ret = NULL;

	ENSURE_MEMORY(ERROR, sber);
	ret = load_init(sber, waitq_tryrem(sber->q));
	if (ch && sber->msg)
		*ch = sber->msg->channel;
	return ret;
}

void cn_subscriber_release(CnSubscriber* sber)
{
	ENSURE(sber, ERROR, null_param);
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
	msg = msg_create(dict_data(ch)->broker, args);
	va_end(args);
	msg->channel = ch;
	notify(dict_data(ch), msg);
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
	data = dict_data(ch);
	sber->list = list_ins(sber->list, clist_create(ch));
	mutex_lock(data->mutex);
	data->list = list_ins(data->list, slist_create(sber));
	mutex_unlock(data->mutex);
}
