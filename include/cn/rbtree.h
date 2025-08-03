/**
 * @file cn/rbtree.h
 *
 * @brief Red-black tree.
 */

#ifndef CN_RBTREE_H
#define CN_RBTREE_H

#include <stdint.h>

/**
 * @enum CnBstTrav
 *
 * @brief Traversal type for a binary search tree.
 */
enum CnBstTrav {
	CN_BST_TRAV_INORDER = 0, /**< BST inorder traversal.   */
	CN_BST_TRAV_PREORDER,    /**< BST preorder traversal.  */
	CN_BST_TRAV_POSTORDER,   /**< BST postorder traversal. */
};

/**
 * @struct CnRbnode
 *
 * @brief Red-black tree node.
 */
struct CnRbnode {
	/**
	 * @var u
	 *
	 * @brief The parent pointer and the node color.
	 */
	union {
		/**
		 * @var intptr_t parcol
		 *
		 * @brief The parent pointer and the node color.
		 */
		intptr_t parcol;

		/**
		 * @var intptr_t align
		 *
		 * @brief Alignment.
		 */
		int32_t align;
	} u;

	/**
	 * @var struct CnRbnode* left
	 *
	 * @brief The pointer to the left child.
	 */
	struct CnRbnode* left;

	/**
	 * @var struct CnRbnode* right
	 *
	 * @brief The pointer to the right child.
	 */
	struct CnRbnode* right;
};

/**
 * @fn struct CnRbnode* cn_rb_link(struct CnRbnode* node, struct CnRbnode* parent)
 * @brief *** todo ***.
 * @param[in,out] node Input/output.
 * @param[in,out] parent Input/output.
 * @return *** todo ***.
 */
struct CnRbnode* cn_rb_link(struct CnRbnode* node, struct CnRbnode* parent);

/**
 * @fn struct CnRbnode* cn_rb_insrebal(struct CnRbnode* root, struct CnRbnode* node)
 * @brief *** todo ***.
 * @param[in,out] root Input/output.
 * @param[in,out] node Input/output.
 * @return *** todo ***.
 */
struct CnRbnode* cn_rb_insrebal(struct CnRbnode* root, struct CnRbnode* node);

/**
 * @fn struct CnRbnode* cn_rb_parent(struct CnRbnode* node)
 * @brief *** todo ***.
 * @param[in] node Input/output.
 * @return *** todo ***.
 */
struct CnRbnode* cn_rb_parent(struct CnRbnode* node);

/**
 * @fn struct CnRbnode* cn_rb_deepest(struct CnRbnode* node)
 * @brief *** todo ***.
 * @param[in] node Input/output.
 * @return *** todo ***.
 */
struct CnRbnode* cn_rb_deepest(struct CnRbnode* node);

/**
 * @fn struct CnRbnode* cn_rb_smallest(struct CnRbnode* node)
 * @brief *** todo ***.
 * @param[in] node Input/output.
 * @return *** todo ***.
 */
struct CnRbnode* cn_rb_smallest(struct CnRbnode* node);

/**
 * @fn struct CnRbnode* cn_rb_next(struct CnRbnode* node, enum CnBstTrav trav)
 * @brief *** todo ***.
 * @param[in] node Input/output.
 * @param[in] trav Input/output.
 * @return *** todo ***.
 */
struct CnRbnode* cn_rb_next(struct CnRbnode* node, enum CnBstTrav trav);

#endif /* CN_RBTREE_H */
