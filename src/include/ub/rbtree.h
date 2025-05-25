#ifndef UB_RBTREE_H
#define UB_RBTREE_H

#include <stdint.h>

struct UBrbnode {
	union {
		intptr_t parcol;
		int32_t align;
	} u;
	struct UBrbnode* left;
	struct UBrbnode* right;
};

struct UBrbnode* ub_rb_link(struct UBrbnode* node, struct UBrbnode* parent);

struct UBrbnode* ub_rb_insrebal(struct UBrbnode* root, struct UBrbnode* node);

struct UBrbnode* ub_rb_parent(struct UBrbnode* node);

struct UBrbnode* ub_rb_deepest(struct UBrbnode* node);

struct UBrbnode* ub_rb_preorder(struct UBrbnode* node);

#endif /* UB_RBTREE_H */
