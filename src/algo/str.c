#include "cantil/str.h"
#include "cantil/arith.h"
#include "cantil/cirq.h"
#include "cantil/dict.h"
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

	ENSURE_MEMORY(self, ERROR);
	*list_data(self) = str;
	return list_ins(list, self);
}

char* cn_newstr(const char* str)
{
	return strcpy(NEW(char, strlen(str) + 1), str);
}

char* cn_strlist_rem(struct CnStrlist** listp)
{
	struct CnStrlist* tmp = list_rem(listp);
	char* ret = *list_data(tmp);

	cn_free(tmp);
	return ret;
}

struct CnStrq* cn_strq_ins(struct CnStrq* q, char* str)
{
	struct CnStrq* self = NEW(struct CnStrq);

	ENSURE_MEMORY(self, ERROR);
	*cirq_data(self) = str;
	return cirq_ins(q, self);
}

char* cn_strq_rem(struct CnStrq** qp)
{
	struct CnStrq* tmp = NULL;
	char* ret = NULL;

	ENSURE_MEMORY(qp, ERROR);
	tmp = cirq_rem(qp);
	ret = *cirq_data(tmp);
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
		i = rb_deepest(&dict_cast(bag)->node);
		p = rb_parent(i);
		bag_destroy(container_of(
			strnode_from(i), struct CnStrbag, strnode));
		if (!p)
			break;

		if (i == p->left)
			p->left = NULL;
		else
			p->right = NULL;
	}
}
