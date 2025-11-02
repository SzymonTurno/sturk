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
 * @file vertegs/rbtree.h
 *
 * @brief Red-black tree.
 *
 * This header file provides data types, functions and macros that define and
 * operate on red-black trees.
 */

#ifndef VERTEGS_RBTREE_H
#define VERTEGS_RBTREE_H

#include "vertegs/graph.h"

/**
 * @def VX_RBTREE(name, type)
 *
 * @brief Define the red-black tree.
 *
 * @param[in] name The name of the type used for the list.
 * @param[in] type The type of the data held by @a name.
 *
 * This macro will define a compound type (must be struct or union) @a name,
 * a type for a red-black tree entry that holds the data of the type @a type.
 */
#define VX_RBTREE(name, type) VX_GRAPH(name, VX_RB_N_EDGES, type)

/**
 * @def vx_rb_link(node, parent)
 *
 * @brief Prepare a node for insertion in a red-black tree.
 *
 * @param[in,out] node The node prepared for insertion.
 * @param[in] parent The parent node.
 *
 * This will paint @a node red and set its parent pointer to @a parent. This
 * will also ensure that the left and right pointers of @a node are NULL. After
 * preparing the node for insertion, insert it with vx_rb_insrebal().
 *
 * @see vx_rb_insrebal().
 *
 * @return The prepared node.
 */
#define vx_rb_link(node, parent)                                               \
	(vx_graph_4vx(                                                         \
		vx_linkrb(vx_graph_2vx(node), vx_graph_2vx(parent)), (node)))

/**
 * @def vx_rb_unlink(node)
 *
 * @brief In a red-black tree set to null the pointer that stores the address of the given node.
 *
 * @param[in] node The given node.
 */
#define vx_rb_unlink(node) (vx_unlinkrb(vx_graph_2vx(node)))

/**
 * @def vx_rb_insrebal(root, node)
 *
 * @brief Insert a node in a red-black tree and rebalance the tree.
 *
 * @param[in,out] root The root of the red-black tree.
 * @param[in,out] node The inserted node.
 *
 * @return The new root.
 *
 * @note Prepare @a node with vx_rb_link() before insertion.
 *
 * @see vx_rb_link().
 */
#define vx_rb_insrebal(root, node)                                             \
	(vx_graph_4vx(                                                         \
		vx_insrebalrb(vx_graph_2vx(root), vx_graph_2vx(node)),         \
		(node)))

/**
 * @def vx_rb_deepest(node)
 *
 * @brief Get the first node of a post-order tree traversal.
 *
 * @param[in] node The node from the tree.
 *
 * @return The first node.
 */
#define vx_rb_deepest(node)                                                    \
	(vx_graph_4vx(vx_postorderfirst(vx_graph_2vx(node)), (node)))

/**
 * @def vx_rb_root(node)
 *
 * @brief Get the of a red-black tree.
 *
 * @param[in] node The node from the tree.
 *
 * @return The root.
 */
#define vx_rb_root(node)                                                       \
	(vx_graph_4vx(vx_walksafe(graph_2vx(node), VX_RB_PARCOL, -1), node))

/**
 * @def vx_rb_first(node)
 *
 * @brief Get the first node of an in-order tree traversal.
 *
 * @param[in] node The node from the tree.
 *
 * @return The first node.
 */
#define vx_rb_first(node)                                                      \
	(vx_graph_4vx(vx_inorderfirst(vx_graph_2vx(node)), (node)))

/**
 * @def vx_rb_next(node)
 *
 * @brief Get the next node of an in-order tree traversal.
 *
 * @param[in] node The node from the tree.
 *
 * @return The next node.
 */
#define vx_rb_next(node)                                                       \
	(vx_graph_4vx(vx_inordernext(vx_graph_2vx(node)), (node)))

#endif /* VERTEGS_RBTREE_H */
