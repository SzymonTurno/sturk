#ifndef MESSAGE_H
#define MESSAGE_H

#include "types.h"

static inline UBload* msg_getload(struct Message* msg)
{
	return (UBload*)msg - ub_dict_data(ub_cirq_data(msg)->chan)->offset;
}

static inline struct Message* msg_create(struct UBchan* chan, va_list args)
{
	struct Chan* c = ub_dict_data(chan);
	UBload* load = ub_pool_tryalloc(c->pool);
	struct Message* self = NULL;
	struct Meta* meta = NULL;

	if (!load) {
		load = ub_pool_alloc(c->pool);
		self = (struct Message*)&load[c->offset];
		meta = ub_cirq_data(self);
		meta->mutex = ub_mutex_create(UB_MUTEX_PROTOCOL_PRIO_INHERIT);
	} else {
		self = (struct Message*)&load[c->offset];
		meta = ub_cirq_data(self);
	}
	meta->chan = chan;
	meta->n_pending = 0;
	c->vp->ctor(load, args);
	return self;
}

static inline void msg_destroy(struct Message* msg)
{
	struct Meta* meta = ub_cirq_data(msg);
	UBchan* chan = meta->chan;

	meta->chan = NULL;
	ub_mutex_destroy(meta->mutex);
	meta->mutex = NULL;
	ub_pool_free(ub_dict_data(chan)->pool, msg_getload(msg));
}

static inline void msg_release(struct Message* msg)
{
	struct Meta* meta = ub_cirq_data(msg);
	int last = 0;

	ub_mutex_lock(meta->mutex);
	if (!--meta->n_pending)
		last = 1;
	ub_mutex_unlock(meta->mutex);
	if (last) {
		ub_dict_data(meta->chan)->vp->dtor(msg_getload(msg));
		ub_pool_free(ub_dict_data(meta->chan)->pool, msg_getload(msg));
	}
}

static inline void msg_lock(struct Message* msg)
{
	ub_mutex_lock(ub_cirq_data(msg)->mutex);
}

static inline void msg_unlock(struct Message* msg, int n_pending)
{
	struct Meta* meta = ub_cirq_data(msg);

	meta->n_pending = n_pending;
	ub_mutex_unlock(meta->mutex);
	if (!n_pending)
		ub_pool_free(ub_dict_data(meta->chan)->pool, msg_getload(msg));
}

#endif /* MESSAGE_H */
