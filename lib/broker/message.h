#ifndef MESSAGE_H
#define MESSAGE_H

#include "types.h"

static inline UBload* msg_getload(struct Message* msg)
{
	return (UBload*)msg - ub_dict_data(msg->chan)->offset;
}

static inline struct Message* msg_create(struct UBchan* chan, va_list args)
{
	struct Chan* c = ub_dict_data(chan);
	UBload* load = ub_pool_tryalloc(c->pool);
	struct Message* self = NULL;

	if (!load) {
		load = ub_pool_alloc(c->pool);
		self = (struct Message*)&load[c->offset];
		self->mutex = ub_mutex_create(UB_MUTEX_POLICY_PRIO_INHERIT);
	} else {
		self = (struct Message*)&load[c->offset];
	}
	self->chan = chan;
	self->n_pending = 0;
	c->vp->ctor(load, args);
	return self;
}

static inline void msg_destroy(struct Message* msg)
{
	UBchan* chan = msg->chan;

	msg->chan = NULL;
	ub_mutex_destroy(msg->mutex);
	msg->mutex = NULL;
	ub_pool_free(ub_dict_data(chan)->pool, msg_getload(msg));
}

static inline void msg_release(struct Message* msg)
{
	int last = 0;

	ub_mutex_lock(msg->mutex);
	if (!--msg->n_pending)
		last = 1;
	ub_mutex_unlock(msg->mutex);
	if (last) {
		ub_dict_data(msg->chan)->vp->dtor(msg_getload(msg));
		ub_pool_free(ub_dict_data(msg->chan)->pool, msg_getload(msg));
	}
}

static inline void msg_lock(struct Message* msg)
{
	ub_mutex_lock(msg->mutex);
}

static inline void msg_unlock(struct Message* msg, int n_pending)
{
	msg->n_pending = n_pending;
	ub_mutex_unlock(msg->mutex);
	if (!n_pending)
		ub_pool_free(ub_dict_data(msg->chan)->pool, msg_getload(msg));
}

#endif /* MESSAGE_H */
