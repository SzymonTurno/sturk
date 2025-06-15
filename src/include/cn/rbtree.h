#ifndef CN_RBTREE_H
#define CN_RBTREE_H

#include <stdint.h>

enum CnBstTrav {
	CN_BST_TRAV_INORDER = 0,
	CN_BST_TRAV_PREORDER,
	CN_BST_TRAV_POSTORDER,
};

struct CnRbnode {
	union {
		intptr_t parcol;
		int32_t align;
	} u;
	struct CnRbnode* left;
	struct CnRbnode* right;
};

struct CnRbnode* cn_rb_link(struct CnRbnode* node, struct CnRbnode* parent);

struct CnRbnode* cn_rb_insrebal(struct CnRbnode* root, struct CnRbnode* node);

struct CnRbnode* cn_rb_parent(struct CnRbnode* node);

struct CnRbnode* cn_rb_deepest(struct CnRbnode* node);

struct CnRbnode* cn_rb_smallest(struct CnRbnode* node);

struct CnRbnode* cn_rb_next(struct CnRbnode* node, enum CnBstTrav trav);

#endif /* CN_RBTREE_H */
