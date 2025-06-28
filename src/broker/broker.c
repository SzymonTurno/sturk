#include "cantil/arith.h"
#include "cantil/logger/except.h"
#include "cantil/logger/trace.h"
#include "cantil/rbtree.h"
#include "cantil/str.h"
#include "cn/os/mem.h"
#include "message.h"

#define MSG_SIZE sizeof(struct Message)

static CnChannel* channel_create(const char* topic, const struct CnLoadVt* vp)
{
	CnChannel* self = cn_malloc(sizeof(*self));
	struct ChannelData* data = dict_data(self);

	dict_setk(self, newstr(topic));
	data->vp = vp;
	data->list = NULL;
	data->mutex = mutex_create(MUTEX_POLICY_PRIO_INHERIT);
	data->offset = (vp->size() / MSG_SIZE + 1) * MSG_SIZE;
	data->pool = pool_create(data->offset + MSG_SIZE);
	return self;
}

static void channel_destroy(CnChannel* ch)
{
	struct ChannelData* data = dict_data(ch);

	ENSURE(!data->list, ERROR, sanity_fail);
	msg_purge(ch);
	pool_destroy(data->pool);
	data->pool = NULL;
	mutex_destroy(data->mutex);
	data->mutex = NULL;
	data->vp = NULL;
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
	struct Qentry* entry = pool_alloc(sber->pool);

	if (!entry) {
		/* LCOV_EXCL_START */
		RAISE(ERROR, null_param);
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
	for (LIST_ITER(struct SubscriberList, i, &data->list)) {
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
	for (LIST_ITER(struct SubscriberList, i, &data->list))
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
	pool_free(sber->pool, q);
	return msg_getload(sber->msg);
}

CnBroker* cn_broker_create(const struct CnLoadVt* vp)
{
	struct CnBroker* self = NULL;

	ENSURE_MEMORY(vp);
	ENSURE_MEMORY(vp->size);
	ENSURE_MEMORY(vp->ctor);
	ENSURE_MEMORY(vp->dtor);
	self = cn_malloc(sizeof(*self));
	self->vp = vp;
	self->dict = NULL;
	self->list = NULL;
	self->mutex = mutex_create(MUTEX_POLICY_PRIO_INHERIT);
	return self;
}

void cn_broker_destroy(CnBroker* broker)
{
	if (!broker)
		return;

	while (broker->list)
		subscriber_destroy(*list_data(broker->list));
	mutex_destroy(broker->mutex);
	broker->mutex = NULL;
	if (broker->dict)
		dict_destroy(broker->dict);
	broker->dict = NULL;
	broker->vp = NULL;
	cn_free(broker);
}

CnChannel* cn_broker_search(CnBroker* broker, const char* topic)
{
	struct CnChannel* ch = NULL;

	ENSURE_MEMORY(broker);
	mutex_lock(broker->mutex);
	ch = dict_find(broker->dict, topic);
	if (!ch) {
		ch = channel_create(topic, broker->vp);
		broker->dict = dict_ins(broker->dict, ch);
	}
	mutex_unlock(broker->mutex);
	return ch;
}

CnSubscriber* cn_subscriber_create(CnBroker* broker)
{
	CnSubscriber* self = NULL;

	ENSURE_MEMORY(broker);
	self = cn_malloc(sizeof(*self));
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
	pool_destroy(sber->pool);
	sber->pool = NULL;
	mutex_lock(sber->broker->mutex);
	for (LIST_ITER(struct SubscriberList, i, &sber->broker->list))
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

	ENSURE_MEMORY(sber);
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

	ENSURE_MEMORY(sber);
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

const char* cn_get_topic(CnChannel* ch)
{
	return dict_getk(ch);
}

void cn_publish(CnChannel* ch, ...)
{
	struct Message* msg = NULL;
	va_list args;

	va_start(args, ch);
	msg = msg_create(ch, args);
	va_end(args);
	notify(dict_data(ch), msg);
}

void cn_subscribe(CnSubscriber* sber, const char* topic)
{
	struct CnChannel* ch = NULL;
	struct ChannelData* data = NULL;

	if (!sber) {
		RAISE(WARNING, null_param);
		return;
	}

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
