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

#ifndef TYPES_H
#define TYPES_H

#include "cantil/broker.h"
#include "cantil/cirq.h"
#include "cantil/dict.h"
#include "cantil/list.h"
#include "cantil/os/mutex.h"
#include "cantil/pool.h"
#include "cantil/waitq.h"

LIST(struct SubscriberList, CnSubscriber*);

CIRQ(struct Qentry, struct Message*);

struct ChannelData {
	CnBroker* broker;
	struct SubscriberList* list;
	CnMutex* mutex;
};

DICT(struct CnChannel, struct ChannelData);

struct CnBroker {
	const struct CnLoadVt* vp;
	CnMutex* mutex;
	struct {
		struct SubscriberList* list;
		CnPool* pool;
	} sbers;
	struct {
		CnChannel* dict;
		CnPool* pool;
	} channels;
};

struct Message {
	CnChannel* channel;
	CnMutex* mutex;
	union {
		int n_pending;
		void* align;
	} u;
};

LIST(struct ChannelList, CnChannel*);

struct CnSubscriber {
	CnBroker* broker;
	CnWaitq* q;
	struct Message* msg;
	struct ChannelList* list;
};

#endif /* TYPES_H */
