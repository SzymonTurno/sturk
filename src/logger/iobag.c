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

#include "sturk/graph.h"
#include "sturk/io/bag.h"
#include "sturk/io/buffer.h"
#include "sturk/list.h"
#include "sturk/os/mem.h"
#include "sturk/os/mutex.h"

LIST(struct IoList, StIo*);

struct StIoBag {
	struct IoList* head;
	StMutex* mutex;
	union {
		size_t n_io;
		void* align;
	} u;
};

static void list_print(struct IoList* head, const char* fmt, va_list va)
{
	va_list vcopy;
	struct IoList* tmp = NULL;

	for (struct IoList** it = &head; *it;) {
		va_copy(vcopy, va);
		tmp = listit_next(&it);
		io_vprint(*graph_datap(tmp), fmt, vcopy);
		va_end(vcopy);
	}
}

StIoBag* st_iobag_create(void)
{
	StIoBag* self = NEW(StIoBag);

	self->head = NULL;
	self->mutex = mutex_create(MUTEX_POLICY_PRIO_INHERIT);
	self->u.n_io = 0;
	return self;
}

void st_iobag_destroy(StIoBag* bag)
{
	if (bag) {
		mutex_destroy(bag->mutex);
		bag->mutex = NULL;
		while (bag->head)
			st_free(list_rem(&bag->head));
		bag->u.n_io = 0;
		st_free(bag);
	}
}

void st_iobag_ins(StIoBag* bag, StIo* io)
{
	struct IoList* entry = NULL;

	VX_ASSERT(bag);
	if (bag) {
		entry = NEW(struct IoList);
		*graph_datap(entry) = io;
		mutex_lock(bag->mutex);
		bag->head = list_ins(bag->head, entry);
		++bag->u.n_io;
		mutex_unlock(bag->mutex);
	}
}

void st_iobag_rem(StIoBag* bag, StIo* io)
{
	VX_ASSERT(bag);
	mutex_lock(bag->mutex);
	for (struct IoList** i = &bag->head; *i; listit_next(&i))
		if (*graph_datap(*i) == io) {
			st_free(list_rem(i));
			--bag->u.n_io;
			break;
		}
	mutex_unlock(bag->mutex);
}

void st_iobag_vprint(StIoBag* bag, const char* fmt, va_list va)
{
	VX_ASSERT(bag);
	if (bag) {
		mutex_lock(bag->mutex);
		list_print(bag->head, fmt, va);
		mutex_unlock(bag->mutex);
	}
}

size_t st_iobag_count(const StIoBag* bag)
{
	return bag ? bag->u.n_io : 0;
}
