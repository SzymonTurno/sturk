/**
 * @file cn/rbtree.h
 * @brief Header file for red-black tree.
 *
 * *** todo ***.
 */

#ifndef CN_RBTREE_H
#define CN_RBTREE_H

#include <stdint.h>

/**
 * @enum CnBstTrav
 * @brief *** todo ***.
 */
enum CnBstTrav {
	CN_BST_TRAV_INORDER = 0, /**< BST inorder traversal.   */
	CN_BST_TRAV_PREORDER,    /**< BST preorder traversal.  */
	CN_BST_TRAV_POSTORDER,   /**< BST postorder traversal. */
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
