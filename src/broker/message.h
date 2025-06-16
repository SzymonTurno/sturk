#ifndef MESSAGE_H
#define MESSAGE_H

#include "cn/os/mem.h"
#include "types.h"

static inline CnLoad* msg_getload(struct Message* msg)
{
	return (CnLoad*)msg - dict_data(msg->channel)->offset;
}

static inline struct Message* msg_create(CnChannel* ch, va_list args)
{
	struct ChannelData* data = dict_data(ch);
	CnLoad* load = pool_tryalloc(data->pool);
	struct Message* self = NULL;

	if (!load) {
		load = pool_alloc(data->pool);
		self = (struct Message*)&load[data->offset];
		self->mutex = mutex_create(MUTEX_POLICY_PRIO_INHERIT);
	} else {
		self = (struct Message*)&load[data->offset];
	}
	self->channel = ch;
	self->u.n_pending = 0;
	data->vp->ctor(load, args);
	return self;
}

static inline void msg_destroy(struct Message* msg)
{
	dict_data(msg->channel)->vp->dtor(msg_getload(msg));
	pool_free(dict_data(msg->channel)->pool, msg_getload(msg));
}

static inline void msg_purge(CnChannel* ch)
{
	struct ChannelData* data = dict_data(ch);
	CnLoad* load = NULL;
	struct Message* msg = NULL;

	while ((load = pool_tryalloc(data->pool))) {
		msg = (struct Message*)&load[data->offset];
		msg->channel = NULL;
		mutex_destroy(msg->mutex);
		msg->mutex = NULL;
		cn_free(load);
	}
}

static inline void msg_release(struct Message* msg)
{
	int last = 0;

	mutex_lock(msg->mutex);
	if (!--msg->u.n_pending)
		last = 1;
	mutex_unlock(msg->mutex);
	if (last)
		msg_destroy(msg);
}

static inline void msg_lock(struct Message* msg)
{
	mutex_lock(msg->mutex);
}

static inline void msg_unlock(struct Message* msg, int n_pending)
{
	msg->u.n_pending = n_pending;
	mutex_unlock(msg->mutex);
	if (!n_pending)
		msg_destroy(msg);
}

#endif /* MESSAGE_H */
