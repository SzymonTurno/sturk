/*
BSD 3-Clause License

Copyright (c) 2025, Szymon Turno

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/**
 * @file cn/rbtree.h
 *
 * @brief Red-black tree.
 */

#ifndef CN_RBTREE_H
#define CN_RBTREE_H

#include "cn/logger/except.h"
#include "vx/graph.h"
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

enum CnRbChild {
	CN_RB_LEFT = 0,   /**< left child.   */
	CN_RB_RIGHT,      /**< right child.  */
	CN_RB_N_CHILDREN, /**< number of children. */
};

/* clang-format off */
VX_GRAPH(struct CnRbnode, CN_RB_N_CHILDREN, union {intptr_t parcol; int32_t align;});
/* clang-format on */

static inline struct CnRbnode* cn_rb_left(struct CnRbnode* node)
{
	struct Vertegs** adjyl = NULL;

	CN_ENSURE_MEM(node, CN_ERROR);
	adjyl = vx_2adjyl(vx_graph_2vx(node));
	return vx_graph_4vx(adjyl[CN_RB_LEFT], node);
}

static inline struct CnRbnode* cn_rb_right(struct CnRbnode* node)
{
	struct Vertegs** adjyl = NULL;

	CN_ENSURE_MEM(node, CN_ERROR);
	adjyl = vx_2adjyl(vx_graph_2vx(node));
	return vx_graph_4vx(adjyl[CN_RB_RIGHT], node);
}

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
