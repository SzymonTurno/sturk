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

#include "sturk/str.h"
#include "sturk/arith.h"
#include "sturk/cirq.h"
#include "sturk/dict.h"
#include "sturk/graph.h"
#include "sturk/list.h"
#include "sturk/logger/except.h"
#include "sturk/logger/trace.h"
#include "sturk/os/mem.h"
#include "sturk/rbtree.h"
#include <string.h>

static struct StStrbag* bag_create(const char* str)
{
	struct StStrbag* self = NEW(struct StStrbag);

	dict_setk(self, newstr(str));
	dict_datap(self)->n = 0;
	return self;
}

static void bag_destroy(struct StStrbag* bag)
{
	st_free(dict_getk(bag));
	dict_setk(bag, NULL);
	st_free(bag);
}

static struct StStrbag* bag_root(struct StStrbag* bag)
{
	struct StRbnode* i = &dict_cast(bag)->node;

	while (st_rb_parent(i))
		i = st_rb_parent(i);
	return container_of(dictnode_from(i), struct StStrbag, dictnode);
}

struct StStrlist* st_strlist_ins(struct StStrlist* list, char* str)
{
	struct StStrlist* self = NEW(struct StStrlist);

	ENSURE_MEM(self, ERROR);
	*graph_datap(self) = str;
	return list_ins(list, self);
}

char* st_newstr(const char* str)
{
	return strcpy(NEW(char, strlen(str) + 1), str);
}

char* st_strlist_rem(struct StStrlist** listp)
{
	struct StStrlist* tmp = NULL;
	char* ret = NULL;

	ENSURE_MEM(listp, ERROR);
	tmp = list_rem(listp);
	ret = *graph_datap(tmp);
	st_free(tmp);
	return ret;
}

struct StStrq* st_strq_ins(struct StStrq* q, char* str)
{
	struct StStrq* self = NEW(struct StStrq);

	ENSURE_MEM(self, ERROR);
	*graph_datap(self) = str;
	return cirq_ins(q, self);
}

char* st_strq_rem(struct StStrq** qp)
{
	struct StStrq* tmp = NULL;
	char* ret = NULL;

	ENSURE_MEM(qp, ERROR);
	ENSURE_MEM(*qp, ERROR);
	tmp = cirq_rem(qp);
	ret = *graph_datap(tmp);
	st_free(tmp);
	return ret;
}

struct StStrbag* st_strbag_ins(struct StStrbag* bag, const char* str)
{
	struct StStrbag* p = dict_find(bag, str);

	if (!p) {
		p = bag_create(str);
		bag = dict_ins(bag, p);
	}
	++dict_datap(p)->n;
	return bag;
}

struct StStrbag* st_strbag_rem(struct StStrbag* bag, const char* str)
{
	struct StStrbag* p = dict_find(bag, str);

	if (!p) {
		p = bag_create(str);
		bag = dict_ins(bag, p);
	}
	--dict_datap(p)->n;
	return bag;
}

int st_strbag_count(const struct StStrbag* bag)
{
	return bag ? dict_datap(bag)->n : 0;
}

void st_strbag_destroy(struct StStrbag* bag)
{
	if (!bag)
		return;
	bag = bag_root(bag);
	for (struct StRbnode *i = NULL, *p = NULL;;) {
		i = rb_first(&dict_cast(bag)->node, BST_POSTORDER);
		p = rb_parent(i);
		bag_destroy(container_of(
			dictnode_from(i), struct StStrbag, dictnode));
		if (!p)
			break;

		if (i == rb_left(p))
			graph_2vx(p)->nbor[BST_LEFT] = NULL;
		else
			graph_2vx(p)->nbor[BST_RIGHT] = NULL;
	}
}
