#ifndef UB_RBTREE_H
#define UB_RBTREE_H

#include <stdint.h>

enum CnBstTrav {
	CN_BST_TRAV_INORDER = 0,
	CN_BST_TRAV_PREORDER,
	CN_BST_TRAV_POSTORDER,
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

struct UBrbnode* ub_rb_next(struct UBrbnode* node, enum CnBstTrav trav);

#endif /* UB_RBTREE_H */
