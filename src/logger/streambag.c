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

#include "sturk/logger/streambag.h"
#include "sturk/graph.h"
#include "sturk/list.h"
#include "sturk/logger/except.h"
#include "sturk/logger/trace.h"
#include "sturk/os/mem.h"
#include "sturk/os/mutex.h"

LIST(struct StreamList, StFstream*);

struct StStreamBag {
	struct StreamList* head;
	StMutex* mutex;
	union {
		int n_streams;
		void* align;
	} u;
};

static void
list_print(struct StreamList* head, const char* format, va_list vlist)
{
	va_list vcopy;
	struct StreamList* tmp = NULL;

	for (struct StreamList** it = &head; *it;) {
		va_copy(vcopy, vlist);
		tmp = listit_next(it);
		st_vfprintf(*graph_datap(tmp), format, vcopy);
		va_end(vcopy);
	}
}

StStreamBag* st_streambag_create(void)
{
	StStreamBag* self = NEW(StStreamBag);

	self->head = NULL;
	self->mutex = mutex_create(MUTEX_POLICY_PRIO_INHERIT);
	self->u.n_streams = 0;
	return self;
}

void st_streambag_destroy(StStreamBag* bag)
{
	if (!bag)
		return;
	mutex_destroy(bag->mutex);
	bag->mutex = NULL;
	while (bag->head)
		st_free(list_rem(&bag->head));
	st_free(bag);
}

void st_streambag_ins(StStreamBag* bag, StFstream* stream)
{
	struct StreamList* entry = NEW(struct StreamList);

	ENSURE(bag, ERROR, null_param);
	*graph_datap(entry) = stream;
	mutex_lock(bag->mutex);
	bag->head = list_ins(bag->head, entry);
	++bag->u.n_streams;
	mutex_unlock(bag->mutex);
}

void st_streambag_rem(StStreamBag* bag, StFstream* stream)
{
	ENSURE(bag, ERROR, null_param);
	list_foreach (struct StreamList, i, &bag->head)
		if (*graph_datap(*i) == stream) {
			mutex_lock(bag->mutex);
			st_free(list_rem(i));
			--bag->u.n_streams;
			mutex_unlock(bag->mutex);
			break;
		}
}

void st_streambag_vprint(StStreamBag* bag, const char* format, va_list vlist)
{
	ENSURE(bag, ERROR, null_param);
	mutex_lock(bag->mutex);
	list_print(bag->head, format, vlist);
	mutex_unlock(bag->mutex);
}

int st_streambag_count(const StStreamBag* bag)
{
	return bag ? bag->u.n_streams : 0;
}
