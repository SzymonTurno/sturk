#ifndef UB_RBTREE_H
#define UB_RBTREE_H

#include <stdint.h>

enum CyBstTrav {
	CY_BST_TRAV_INORDER = 0,
	CY_BST_TRAV_PREORDER,
	CY_BST_TRAV_POSTORDER,
};

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

struct UBrbnode* ub_rb_leftmost(struct UBrbnode* node);

struct UBrbnode* ub_rb_next(struct UBrbnode* node, enum CyBstTrav trav);

#endif /* UB_RBTREE_H */
