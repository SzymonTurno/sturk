#ifndef MESSAGE_H
#define MESSAGE_H

#include "types.h"
#include "ub/os/mem.h"

static inline UBload* msg_getload(struct Message* msg)
{
	return (UBload*)msg - dict_data(msg->chan)->offset;
}

static inline struct Message* msg_create(struct UBchan* chan, va_list args)
{
	struct Chan* c = dict_data(chan);
	UBload* load = pool_tryalloc(c->pool);
	struct Message* self = NULL;

	if (!load) {
		load = pool_alloc(c->pool);
		self = (struct Message*)&load[c->offset];
		self->mutex = mutex_create(MUTEX_POLICY_PRIO_INHERIT);
	} else {
		self = (struct Message*)&load[c->offset];
	}
	self->chan = chan;
	self->u.n_pending = 0;
	c->vp->ctor(load, args);
	return self;
}

static inline void msg_purge(struct UBchan* chan)
{
	struct Chan* c = dict_data(chan);
	UBload* load = NULL;
	struct Message* msg = NULL;

	while ((load = pool_tryalloc(c->pool))) {
		msg = (struct Message*)&load[c->offset];
		msg->chan = NULL;
		mutex_destroy(msg->mutex);
		msg->mutex = NULL;
		ub_free(load);
	}
}

static inline void msg_release(struct Message* msg)
{
	int last = 0;

	mutex_lock(msg->mutex);
	if (!--msg->u.n_pending)
		last = 1;
	mutex_unlock(msg->mutex);
	if (last) {
		dict_data(msg->chan)->vp->dtor(msg_getload(msg));
		pool_free(dict_data(msg->chan)->pool, msg_getload(msg));
	}
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
		pool_free(dict_data(msg->chan)->pool, msg_getload(msg));
}

#endif /* MESSAGE_H */
