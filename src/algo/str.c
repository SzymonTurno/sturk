#include "UB/str.h"
#include "UB/arith.h"
#include "UB/rbtree.h"
#include "ub/os/mem.h"
#include <string.h>

static struct CyStrbag* bag_create(const char* str)
{
	struct CyStrbag* self = ub_malloc(sizeof(*self));

	dict_setk(self, newstr(str));
	dict_data(self)->n = 0;
	return self;
}

static void bag_destroy(struct CyStrbag* bag)
{
	ub_free(dict_getk(bag));
	dict_setk(bag, NULL);
	ub_free(bag);
}

struct CyStrlist* cy_strlist_ins(struct CyStrlist* list, char* str)
{
	struct CyStrlist* self = ub_malloc(sizeof(*self));

	*list_data(self) = str;
	return list_ins(list, self);
}

char* cy_newstr(const char* str)
{
	return strcpy(ub_malloc(strlen(str) + 1), str);
}

char* cy_strlist_rem(struct CyStrlist** listp)
{
	struct CyStrlist* tmp = list_rem(listp);
	char* ret = *list_data(tmp);

	ub_free(tmp);
	return ret;
}

struct CyStrq* cy_strq_ins(struct CyStrq* q, char* str)
{
	struct CyStrq* self = ub_malloc(sizeof(*self));

	*cirq_data(self) = str;
	return cirq_ins(q, self);
}

char* cy_strq_rem(struct CyStrq** qp)
{
	struct CyStrq* tmp = cirq_rem(qp);
	char* ret = *cirq_data(tmp);

	ub_free(tmp);
	return ret;
}

struct CyStrbag* cy_strbag_ins(struct CyStrbag* bag, const char* str)
{
	struct CyStrbag* p = dict_find(bag, str);

	if (!p) {
		p = bag_create(str);
		bag = dict_ins(bag, p);
	}
	++dict_data(p)->n;
	return bag;
}

struct CyStrbag* cy_strbag_rem(struct CyStrbag* bag, const char* str)
{
	struct CyStrbag* p = dict_find(bag, str);

	if (!p) {
		p = bag_create(str);
		bag = dict_ins(bag, p);
	}
	--dict_data(p)->n;
	return bag;
}

int cy_strbag_count(struct CyStrbag* bag)
{
	return bag ? dict_data(bag)->n : 0;
}

void cy_strbag_destroy(struct CyStrbag* bag)
{
	for (struct UBrbnode *i = NULL, *p = NULL;;) {
		i = rb_deepest(&dict_cast(bag)->node);
		p = rb_parent(i);
		bag_destroy(
			container_of(strnode_from(i), struct CyStrbag,
				strnode));
		if (!p)
			break;

		if (i == p->left)
			p->left = NULL;
		else
			p->right = NULL;
	}
}
