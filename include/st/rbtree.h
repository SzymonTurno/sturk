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
 * @file st/rbtree.h
 *
 * @brief Red-black tree.
 */

#ifndef ST_RBTREE_H
#define ST_RBTREE_H

#include "st/logger/except.h"
#include "vx/graph.h"
#include <stdint.h>

/**
 * @enum StBstOrder
 *
 * @brief Nodes order for a binary search tree.
 */
enum StBstOrder {
	ST_BST_INORDER = 0, /**< in-order.   */
	ST_BST_PREORDER,    /**< pre-order.  */
	ST_BST_POSTORDER,   /**< post-order. */
};

/**
 * @enum StBstChild
 *
 * @brief Indices for children in a binary search tree.
 */
enum StBstChild {
	ST_BST_LEFT = 0,   /**< left child.   */
	ST_BST_RIGHT,      /**< right child.  */
	ST_BST_N_CHILDREN, /**< number of children. */
};

/* clang-format off */
/**
 * @def VX_GRAPH(struct StRbnode, ST_BST_N_CHILDREN, union {intptr_t parcol; int32_t align;})
 *
 * @struct StRbNode
 *
 * @brief Node of a red-black tree.
 */
VX_GRAPH(struct StRbnode, ST_BST_N_CHILDREN, union {intptr_t parcol; int32_t align;});
/**< This is a macro definition of the struct StRbnode type. */
/* clang-format on */

/**
 * @fn static inline struct StRbnode* st_rb_left(struct StRbnode* node)
 *
 * @brief Get the left child of the given node.
 *
 * @param[in] node The node.
 *
 * @return The left child.
 */
static inline struct StRbnode* st_rb_left(struct StRbnode* node)
{
	ST_ENSURE_MEM(node, ST_ERROR);
	return vx_graph_4vx(vx_graph_2vx(node)->nbor[ST_BST_LEFT], node);
}

/**
 * @fn static inline struct StRbnode* st_rb_right(struct StRbnode* node)
 *
 * @brief Get the right child of the given node.
 *
 * @param[in] node The node.
 *
 * @return The right child.
 */
static inline struct StRbnode* st_rb_right(struct StRbnode* node)
{
	ST_ENSURE_MEM(node, ST_ERROR);
	return vx_graph_4vx(vx_graph_2vx(node)->nbor[ST_BST_RIGHT], node);
}

/**
 * @fn struct StRbnode* st_rb_link(struct StRbnode* node, struct StRbnode* parent)
 *
 * @brief Prepare a node for insertion in a red-black tree.
 *
 * @param[in,out] node The node prepared for insertion.
 * @param[in] parent The parent node.
 *
 * This will paint @a node red and set its parent pointer to @a parent. This
 * will also ensure that the left and right pointers of @a node are NULL. After
 * preparing the node for insertion, insert it with st_rb_insrebal().
 * @see st_rb_insrebal().
 *
 * @return The prepared node.
 */
struct StRbnode* st_rb_link(struct StRbnode* node, struct StRbnode* parent);

/**
 * @fn struct StRbnode* st_rb_insrebal(struct StRbnode* root, struct StRbnode* node)
 *
 * @brief Insert a node in a red-black tree and rebalance the tree.
 *
 * @param[in,out] root The root of the red-black tree.
 * @param[in,out] node The inserted node.
 *
 * @return The new root.
 *
 * @note Prepare @a node with st_rb_link() before insertion.
 * @see st_rb_link().
 */
struct StRbnode* st_rb_insrebal(struct StRbnode* root, struct StRbnode* node);

/**
 * @fn struct StRbnode* st_rb_parent(struct StRbnode* node)
 *
 * @brief Get the parent of the red-black tree node.
 *
 * @param[in] node The red-black tree node.
 *
 * @return The parent.
 */
struct StRbnode* st_rb_parent(struct StRbnode* node);

/**
 * @fn struct StRbnode* st_rb_first(struct StRbnode* node, enum StBstOrder order)
 *
 * @brief Get the first node of the red-black tree for the given BST order.
 *
 * @param[in] node Any node from the red-black tree.
 * @param[in] order The order: BST_INORDER, BST_PREORDER or BST_POSTORDER.
 *
 * @return The first node.
 */
struct StRbnode* st_rb_first(struct StRbnode* node, enum StBstOrder order);

/**
 * @fn struct StRbnode* st_rb_next(struct StRbnode* node, enum StBstOrder order)
 *
 * @brief Get the next node for the current red-black tree node and the BST order.
 *
 * @param[in] node The current red-black tree node.
 * @param[in] order The order: BST_INORDER, BST_PREORDER or BST_POSTORDER.
 *
 * @return The next node.
 */
struct StRbnode* st_rb_next(struct StRbnode* node, enum StBstOrder order);

#endif /* ST_RBTREE_H */
