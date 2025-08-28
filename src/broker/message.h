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

#ifndef MESSAGE_H
#define MESSAGE_H

#include "cn/os/mem.h"
#include "types.h"

static inline CnLoad* msg_2load(struct Message* msg)
{
	ENSURE(msg, ERROR, sanity_fail);
	return (CnLoad*)&msg[1];
}

static inline const struct Message* msg_4load(const CnLoad* load)
{
	ENSURE(load, ERROR, sanity_fail);
	return ((const struct Message*)load) - 1;
}

static inline struct Message* msg_create(CnBroker* broker, va_list args)
{
	struct Message* self = NULL;

	ENSURE(broker, ERROR, sanity_fail);
	self = pool_tryalloc(broker->channels.pool);
	if (!self) {
		self = pool_alloc(broker->channels.pool);
		self->mutex = mutex_create(MUTEX_POLICY_PRIO_INHERIT);
	}
	self->u.n_pending = 0;
	broker->vp->ctor(msg_2load(self), args);
	return self;
}

static inline void msg_destroy(struct Message* msg)
{
	CnBroker* broker = NULL;

	ENSURE(msg, ERROR, sanity_fail);
	broker = dict_datap(msg->channel)->broker;
	broker->vp->dtor(msg_2load(msg));
	pool_free(broker->channels.pool, msg);
}

static inline void msg_purge(CnBroker* broker)
{
	struct Message* msg = NULL;

	ENSURE(broker, ERROR, sanity_fail);
	while ((msg = pool_tryalloc(broker->channels.pool))) {
		msg->channel = NULL;
		mutex_destroy(msg->mutex);
		msg->mutex = NULL;
		cn_free(msg);
	}
}

static inline void msg_release(struct Message* msg)
{
	int last = 0;

	ENSURE(msg, ERROR, sanity_fail);
	ENSURE(msg->u.n_pending, ERROR, sanity_fail);
	mutex_lock(msg->mutex);
	last = (msg->u.n_pending == 1);
	--msg->u.n_pending;
	mutex_unlock(msg->mutex);
	if (last)
		msg_destroy(msg);
}

static inline void msg_lock(struct Message* msg)
{
	ENSURE(msg, ERROR, sanity_fail);
	mutex_lock(msg->mutex);
}

static inline void msg_unlock(struct Message* msg, int n_pending)
{
	ENSURE(msg, ERROR, sanity_fail);
	msg->u.n_pending = n_pending;
	mutex_unlock(msg->mutex);
	if (!n_pending)
		msg_destroy(msg);
}

#endif /* MESSAGE_H */
