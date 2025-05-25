#include "ub/multiset.h"
#include "ub/os/mem.h"
#include "UB/misc.h"
#include "UB/dict.h"
#include "UB/rbtree.h"

DICT(struct CyMultiset, union { int n; void* align; });

static CyMultiset* create(const char* str)
{
	CyMultiset* self = ub_malloc(sizeof(*self));

	dict_setk(self, newstr(str));
	dict_data(self)->n = 0;
	return self;
}

static void destroy(CyMultiset* mset)
{
	ub_free(dict_getk(mset));
	dict_setk(mset, NULL);
	ub_free(mset);
}

CyMultiset* cy_multiset_ins(CyMultiset* mset, const char* str)
{
	CyMultiset* p = dict_find(mset, str);

	if (!p) {
		p = create(str);
		mset = dict_ins(mset, p);
	}
	++dict_data(p)->n;
	return mset;
}

CyMultiset* cy_multiset_rem(CyMultiset* mset, const char* str)
{
	CyMultiset* p = dict_find(mset, str);

	if (!p) {
		p = create(str);
		mset = dict_ins(mset, p);
	}
	--dict_data(p)->n;
	return mset;
}

const char* cy_multiset_getk(CyMultiset* mset)
{
	return dict_getk(mset);
}

int cy_multiset_count(CyMultiset* mset)
{
	return mset ? dict_data(mset)->n : 0;
}

CyMultiset* cy_multiset_find(CyMultiset* mset, const char* str)
{
	return dict_find(mset, str);
}

CyMultiset* cy_multiset_next(CyMultiset* mset)
{
	return dict_preorder(mset, CyMultiset);
}

void cy_multiset_destroy(CyMultiset* mset)
{
	for (struct UBrbnode *i = NULL, *p = NULL;;) {
		i = rb_deepest(&dict_cast(mset)->node);
		p = rb_parent(i);
		destroy(dict_cont(strnode_cont(i), CyMultiset));
		if (!p)
			break;

		if (i == p->left)
			p->left = NULL;
		else
			p->right = NULL;
	}
}
