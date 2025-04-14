#include "ub/dict.h"
#include "UB/arith.h"
#include "UB/logger.h"
#include "UB/rbtree.h"
#include <string.h>

static struct UBstrnode* cont(struct UBrbnode* ptr)
{
	return container_of(ptr, struct UBstrnode, node);
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

	ENSURE(node && node->str, "Null pointer.");
	while (*i) {
		p = cont(*i);
		if (strcmp(node->str, p->str) < 0)
			i = &p->node.left;
		else
			i = &p->node.right;
	}
	*i = rb_link(&node->node, p ? &p->node : NULL);
	return cont(rb_insrebal(tmp, &node->node));
}

struct UBstrnode* ub_strnode_find(struct UBstrnode* root, const char* str)
{
	int tmp = 0;

	ENSURE(str, "Null pointer.");
	while (root) {
		ENSURE(root->str, "Null pointer.");
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
