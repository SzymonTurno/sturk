#include "defs.h"
#include "ub/dict.h"
#include "ub/os/mem.h"

UB_DICT(struct Dict, struct UBchan);

extern struct Dict* dict_ins(struct Dict* dict, const char* topic);
extern void dict_destroy(struct Dict* dict);

static struct Dict* create_entry(const char* topic)
{
	struct Dict* self = ub_malloc(sizeof(*self));

	ub_dict_setkey(self, strcpy(ub_malloc(strlen(topic) + 1), topic));
	*ub_dict_cast(self) = (struct UBchan){0};
	return self;
}

static void destroy_entry(struct Dict* entry)
{
	ub_free(ub_dict_getkey(entry));
	ub_free(entry);
}

struct Dict* dict_ins(struct Dict* dict, const char* topic)
{
	if (ub_dict_find(dict, topic))
		return dict;
	return ub_dict_ins(dict, create_entry(topic));
}

void dict_destroy(struct Dict* dict)
{
	for (struct UBrbnode *i = NULL, *p = NULL;;) {
		i = ub_rb_deepest((struct UBrbnode*)dict);
		p = ub_rb_parent(i);
		destroy_entry((struct Dict*)i);
		if (!p)
			break;

		if (i == p->left)
			p->left = NULL;
		else
			p->right = NULL;
	}
}
