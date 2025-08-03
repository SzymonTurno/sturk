/**
 * @file cn/rbtree.h
 *
 * @brief Red-black tree.
 */

#ifndef CN_RBTREE_H
#define CN_RBTREE_H

#include <stdint.h>

/**
 * @enum CnBstOrder
 *
 * @brief Nodes order for a binary search tree.
 */
enum CnBstOrder {
	CN_BST_INORDER = 0, /**< in-order.   */
	CN_BST_PREORDER,    /**< pre-order.  */
	CN_BST_POSTORDER,   /**< post-order. */
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
 *
 * @brief Prepare a node for insertion in a red-black tree.
 *
 * @param[in,out] node The node prepared for insertion.
 * @param[in] parent The parent node.
 *
 * This will paint @a node red and set its parent pointer to @a parent. This
 * will also ensure that the left and right pointers of @a node are NULL. After
 * preparing the node for insertion, insert it with cn_rb_insrebal().
 * @see cn_rb_insrebal().
 *
 * @return The prepared node.
 */
struct CnRbnode* cn_rb_link(struct CnRbnode* node, struct CnRbnode* parent);

/**
 * @fn struct CnRbnode* cn_rb_insrebal(struct CnRbnode* root, struct CnRbnode* node)
 *
 * @brief Insert a node in a red-black tree and rebalance the tree.
 *
 * @param[in,out] root The root of the red-black tree.
 * @param[in,out] node The inserted node.
 *
 * @return The new root.
 *
 * @note Prepare @a node with cn_rb_link() before insertion.
 * @see cn_rb_link().
 */
struct CnRbnode* cn_rb_insrebal(struct CnRbnode* root, struct CnRbnode* node);

/**
 * @fn struct CnRbnode* cn_rb_parent(struct CnRbnode* node)
 *
 * @brief Get the parent of the red-black tree node.
 *
 * @param[in] node The red-black tree node.
 *
 * @return The parent.
 */
struct CnRbnode* cn_rb_parent(struct CnRbnode* node);

/**
 * @fn struct CnRbnode* cn_rb_first(struct CnRbnode* node, enum CnBstOrder order)
 *
 * @brief Get the first node of the red-black tree for the given BST order.
 *
 * @param[in] node Any node from the red-black tree.
 * @param[in] order The order: BST_INORDER, BST_PREORDER or BST_POSTORDER.
 *
 * @return The first node.
 */
struct CnRbnode* cn_rb_first(struct CnRbnode* node, enum CnBstOrder order);

/**
 * @fn struct CnRbnode* cn_rb_next(struct CnRbnode* node, enum CnBstOrder order)
 *
 * @brief Get the next node for the current red-black tree node and the BST order.
 *
 * @param[in] node The current red-black tree node.
 * @param[in] order The order: BST_INORDER, BST_PREORDER or BST_POSTORDER.
 *
 * @return The next node.
 */
struct CnRbnode* cn_rb_next(struct CnRbnode* node, enum CnBstOrder order);

#endif /* CN_RBTREE_H */
