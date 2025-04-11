#include "ub/dict.h"
#include <string.h>

static struct UBstrnode* cont(struct UBrbnode* ptr)
{
	return ub_container_of(ptr, struct UBstrnode, node);
}

static struct UBstrnode* trycont(struct UBrbnode* ptr)
{
	return ptr ? cont(ptr) : NULL;
}

struct UBstrnode*
ub_strnode_ins(struct UBstrnode* root, struct UBstrnode* node)
{
	struct UBrbnode* tmp = root ? &root->node : NULL;
	struct UBrbnode** i = &tmp;
	struct UBstrnode* p = NULL;

	UB_ENSURE(node && node->str, "Null pointer.");
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

	UB_ENSURE(str, "Null pointer.");
	while (root) {
		UB_ENSURE(root->str, "Null pointer.");
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
