#include "ub/dict.h"
#include <string.h>

#define cont(ptr) ub_container_of(ptr, struct UBstrnode, node)

#define trycont(ptr) ((ptr) ? cont(ptr) : NULL)

struct UBstrnode*
ub_strnode_ins(struct UBstrnode* root, struct UBstrnode* node)
{
	struct UBrbnode* tmp = root ? &root->node : NULL;
	struct UBrbnode** i = &tmp;
	struct UBstrnode* p = NULL;

	ub_ensure(node && node->str, "Null pointer.");
	while (*i) {
		p = cont(*i);
		if (strcmp(node->str, p->str) < 0)
			i = &p->node.left;
		else
			i = &p->node.right;
	}
	*i = ub_rb_link(&node->node, p ? &p->node : NULL);
	return cont(ub_rb_insrebal(tmp, &node->node));
}

struct UBstrnode* ub_strnode_find(struct UBstrnode* root, const char* str)
{
	int tmp = 0;

	ub_ensure(str, "Null pointer.");
	while (root) {
		tmp = strcmp(str, root->str);
		if (tmp < 0)
			root = trycont(root->node.left);
		else if (tmp > 0)
			root = trycont(root->node.right);
		else
			break;
	}
	return root;
}
