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

#include "sturk/broker.h"
#include "sturk/cirq.h"
#include "sturk/dict.h"
#include "sturk/list.h"
#include "sturk/os/mutex.h"
#include "sturk/pool.h"
#include "sturk/waitq.h"

LIST(struct SubscriberList, StSubscriber*);

CIRQ(struct Qentry, struct Message*);

struct ChannelData {
	StBroker* broker;
	struct SubscriberList* list;
	StMutex* mutex;
};

DICT(struct StChannel, struct ChannelData);

struct StBroker {
	const struct StLoadVt* vp;
	StMutex* mutex;
	struct {
		struct SubscriberList* list;
		StPool* pool;
	} sbers;
	struct {
		StChannel* dict;
		StPool* pool;
	} channels;
};

struct Message {
	StChannel* channel;
	StMutex* mutex;
	union {
		int n_pending;
		void* align;
	} u;
};

LIST(struct ChannelList, StChannel*);

struct StSubscriber {
	StBroker* broker;
	StWaitq* q;
	struct Message* msg;
	struct ChannelList* list;
};

#endif /* TYPES_H */
