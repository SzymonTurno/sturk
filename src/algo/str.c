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

#include "cantil/str.h"
#include "cantil/arith.h"
#include "cantil/cirq.h"
#include "cantil/dict.h"
#include "cantil/graph.h"
#include "cantil/list.h"
#include "cantil/logger/except.h"
#include "cantil/logger/trace.h"
#include "cantil/os/mem.h"
#include "cantil/rbtree.h"
#include <string.h>

static struct CnStrbag* bag_create(const char* str)
{
	struct CnStrbag* self = NEW(struct CnStrbag);

	dict_setk(self, newstr(str));
	dict_data(self)->n = 0;
	return self;
}

static void bag_destroy(struct CnStrbag* bag)
{
	cn_free(dict_getk(bag));
	dict_setk(bag, NULL);
	cn_free(bag);
}

static struct CnStrbag* bag_root(struct CnStrbag* bag)
{
	struct CnRbnode* i = &dict_cast(bag)->node;

	while (cn_rb_parent(i))
		i = cn_rb_parent(i);
	return container_of(strnode_from(i), struct CnStrbag, strnode);
}

struct CnStrlist* cn_strlist_ins(struct CnStrlist* list, char* str)
{
	struct CnStrlist* self = NEW(struct CnStrlist);

	ENSURE_MEM(self, ERROR);
	*graph_data(self) = str;
	return list_ins(list, self);
}

char* cn_newstr(const char* str)
{
	return strcpy(NEW(char, strlen(str) + 1), str);
}

char* cn_strlist_rem(struct CnStrlist** listp)
{
	struct CnStrlist* tmp = NULL;
	char* ret = NULL;

	ENSURE_MEM(listp, ERROR);
	tmp = list_rem(listp);
	ret = *graph_data(tmp);
	cn_free(tmp);
	return ret;
}

struct CnStrq* cn_strq_ins(struct CnStrq* q, char* str)
{
	struct CnStrq* self = NEW(struct CnStrq);

	ENSURE_MEM(self, ERROR);
	*graph_data(self) = str;
	return cirq_ins(q, self);
}

char* cn_strq_rem(struct CnStrq** qp)
{
	struct CnStrq* tmp = NULL;
	char* ret = NULL;

	ENSURE_MEM(qp, ERROR);
	ENSURE_MEM(*qp, ERROR);
	tmp = cirq_rem(qp);
	ret = *graph_data(tmp);
	cn_free(tmp);
	return ret;
}

struct CnStrbag* cn_strbag_ins(struct CnStrbag* bag, const char* str)
{
	struct CnStrbag* p = dict_find(bag, str);

	if (!p) {
		p = bag_create(str);
		bag = dict_ins(bag, p);
	}
	++dict_data(p)->n;
	return bag;
}

struct CnStrbag* cn_strbag_rem(struct CnStrbag* bag, const char* str)
{
	struct CnStrbag* p = dict_find(bag, str);

	if (!p) {
		p = bag_create(str);
		bag = dict_ins(bag, p);
	}
	--dict_data(p)->n;
	return bag;
}

int cn_strbag_count(const struct CnStrbag* bag)
{
	return bag ? dict_data(bag)->n : 0;
}

void cn_strbag_destroy(struct CnStrbag* bag)
{
	if (!bag)
		return;
	bag = bag_root(bag);
	for (struct CnRbnode *i = NULL, *p = NULL;;) {
		i = rb_first(&dict_cast(bag)->node, BST_POSTORDER);
		p = rb_parent(i);
		bag_destroy(container_of(
			strnode_from(i), struct CnStrbag, strnode));
		if (!p)
			break;

		if (i == rb_left(p))
			vx_2adjyl(graph_2vx(p))[RB_LEFT] = NULL;
		else
			vx_2adjyl(graph_2vx(p))[RB_RIGHT] = NULL;
	}
}
