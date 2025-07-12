#ifndef MESSAGE_H
#define MESSAGE_H

#include "cn/os/mem.h"
#include "types.h"

static INLINE CnLoad* msg_getload(struct Message* msg)
{
	return (CnLoad*)msg - dict_data(msg->channel)->broker->channels.offset;
}

static INLINE struct Message* msg_create(CnBroker* broker, va_list args)
{
	CnLoad* load = pool_tryalloc(broker->channels.pool);
	struct Message* self = NULL;

	if (!load) {
		load = pool_alloc(broker->channels.pool);
		self = (struct Message*)&load[broker->channels.offset];
		self->mutex = mutex_create(MUTEX_POLICY_PRIO_INHERIT);
	} else {
		self = (struct Message*)&load[broker->channels.offset];
	}
	self->u.n_pending = 0;
	broker->vp->ctor(load, args);
	return self;
}

static INLINE void msg_destroy(struct Message* msg)
{
	CnBroker* broker = dict_data(msg->channel)->broker;

	broker->vp->dtor(msg_getload(msg));
	pool_free(broker->channels.pool, msg_getload(msg));
}

static INLINE void msg_purge(CnBroker* broker)
{
	CnLoad* load = NULL;
	struct Message* msg = NULL;

	while ((load = pool_tryalloc(broker->channels.pool))) {
		msg = (struct Message*)&load[broker->channels.offset];
		msg->channel = NULL;
		mutex_destroy(msg->mutex);
		msg->mutex = NULL;
		cn_free(load);
	}
}

static INLINE void msg_release(struct Message* msg)
{
	int last = 0;

	mutex_lock(msg->mutex);
	if (!--msg->u.n_pending)
		last = 1;
	mutex_unlock(msg->mutex);
	if (last)
		msg_destroy(msg);
}

static INLINE void msg_lock(struct Message* msg)
{
	mutex_lock(msg->mutex);
}

static INLINE void msg_unlock(struct Message* msg, int n_pending)
{
	msg->u.n_pending = n_pending;
	mutex_unlock(msg->mutex);
	if (!n_pending)
		msg_destroy(msg);
}

#endif /* MESSAGE_H */
