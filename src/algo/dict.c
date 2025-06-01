#include "cn/dict.h"
#include "cantil/arith.h"
#include "cantil/logger/except.h"
#include "cantil/rbtree.h"
#include <string.h>

static struct CnStrnode* cont(struct CnRbnode* ptr)
{
	return container_of(ptr, struct CnStrnode, node);
}

static struct CnStrnode* trycont(struct CnRbnode* ptr)
{
	return ptr ? cont(ptr) : NULL;
}

struct CnStrnode*
cn_strnode_ins(struct CnStrnode* root, struct CnStrnode* node)
{
	struct CnRbnode* tmp = root ? &root->node : NULL;
	struct CnRbnode** i = &tmp;
	struct CnStrnode* p = NULL;

	ENSURE(node && node->str, ECODES.null_param);
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

struct CnStrnode* cn_strnode_find(struct CnStrnode* root, const char* str)
{
	int tmp = 0;

	ENSURE(str, ECODES.null_param);
	while (root) {
		ENSURE(root->str, ECODES.null_param);
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
