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
 * @file vertegs/vertex.h
 *
 * @brief Vertex.
 *
 * This header file provides data types, functions and macros that define and
 * operate on graph vertices.
 */

#ifndef VERTEGS_VERTEX_H
#define VERTEGS_VERTEX_H

#include "vertegs/bits.h"
#include "vertegs/debug.h"
#include <stdint.h>

/**
 * @def VX_TRAIT_MASK
 *
 * @brief The bitmask of trait bits.
 */
#define VX_TRAIT_MASK VX_BIT(0)

/**
 * @struct Vertegs
 *
 * @brief The data type for a graph vertex.
 */
struct Vertegs {
	/**
	 * @var struct Vertegs* nbor[1]
	 *
	 * @brief The neighbourhood of the vertex - an array of neighbours.
	 */
	struct Vertegs* nbor[1];
};

/**
 * @enum VxListEdge
 *
 * @brief List edge index.
 */
enum VxListEdge {
	VX_LIST_NEXT = 0, /** edge for the next entry.                        */
	VX_LIST_PREV,     /** edge for the previous entry.                    */
	VX_LIST_N_EDGES   /** number of edges for a doubly linked list entry. */
};

/**
 * @enum VxRbEdge
 *
 * @brief Red-black tree edge index.
 */
enum VxRbEdge {
	VX_RB_LEFT = 0, /** edge for the left node.                     */
	VX_RB_RIGHT,    /** edge for the right node.                    */
	VX_RB_PARCOL,   /** edge for the parent node.                   */
	VX_RB_N_EDGES   /** number of edges for a red-black tree entry. */
};

/**
 * @enum VxRbColor
 *
 * @brief Red-black tree color trait.
 */
enum VxRbColor {
	VX_RB_BLACK = 0, /** black trait.                           */
	VX_RB_RED,       /** red trait.                             */
	VX_RB_N_COLORS   /** number of red-black tree color traits. */
};

/**
 * @fn static inline struct Vertegs* vx_4nbor(struct Vertegs** nbor)
 *
 * @brief Cast to vertex from its neighbourhood.
 *
 * @param[in] nbor The neighbourhood.
 *
 * @return The vertex.
 */
static inline struct Vertegs* vx_4nbor(struct Vertegs** nbor)
{
	return (struct Vertegs*)nbor;
}

/**
 * @fn static inline struct Vertegs* vx_mark(struct Vertegs* v, intptr_t trait)
 *
 * @brief Add a trait to a vertex pointer.
 *
 * @param[in] v The vertex pointer.
 * @param[in] trait The trait bits.
 *
 * This can only be used if the vertex data type address is a multiple of the
 * size of the data type, that is, the pointer is properly aligned.
 *
 * @return The vertex with the trait added.
 */
static inline struct Vertegs* vx_mark(struct Vertegs* v, intptr_t trait)
{
	return (struct Vertegs*)VX_BF_SET((intptr_t)v, VX_TRAIT_MASK, trait);
}

/**
 * @fn static inline struct Vertegs* vx_unmark(struct Vertegs* v)
 *
 * @brief Remove a trait from a vertex pointer.
 *
 * @param[in] v The vertex pointer.
 *
 * @return The vertex with the trait removed.
 */
static inline struct Vertegs* vx_unmark(struct Vertegs* v)
{
	return (struct Vertegs*)((intptr_t)v & ~VX_TRAIT_MASK);
}

/**
 * @fn static inline struct Vertegs* vx_walk(struct Vertegs* v, size_t edge, int len)
 *
 * @brief For a chain, in which all edges share the same index, find the other end.
 *
 * @param[in] v The first end of the chain.
 * @param[in] edge The index of all edges in the chain.
 * @param[in] len The length of the chain - the number of edges.
 *
 * @return The other end of the chain.
 */
static inline struct Vertegs* vx_walk(struct Vertegs* v, size_t edge, int len)
{
	VX_ENSURE_MEM(v);
	while (len-- && v->nbor[edge])
		v = v->nbor[edge];
	return v;
}

/**
 * @fn static inline struct Vertegs* vx_walksafe(struct Vertegs* v, size_t edge, int len)
 *
 * @brief Perform vx_walk() for a graph with traits.
 *
 * @see vx_walk()
 * @see vx_mark()
 *
 * @param[in] v The first end of the chain.
 * @param[in] edge The index of all edges in the chain.
 * @param[in] len The length of the chain - the number of edges.
 *
 * @return The other end of the chain.
 */
static inline struct Vertegs*
vx_walksafe(struct Vertegs* v, size_t edge, int len)
{
	v = vx_unmark(v);
	VX_ENSURE_MEM(v);
	while (len-- && vx_unmark(v->nbor[edge]))
		v = vx_unmark(v->nbor[edge]);
	return v;
}

/**
 * @fn static inline struct Vertegs* vx_inslist(struct Vertegs* list, struct Vertegs* entry, int pos)
 *
 * @brief Insert, at a given position, an entry into a list.
 *
 * @param[in] list The head of the list.
 * @param[in] entry The new entry.
 * @param[in] pos The position.
 *
 * @return The new head.
 */
static inline struct Vertegs*
vx_inslist(struct Vertegs* list, struct Vertegs* entry, int pos)
{
	const size_t next = VX_LIST_NEXT;
	struct Vertegs* nbor[] = {list};
	struct Vertegs* v = vx_walk(vx_4nbor(nbor), next, pos);

	VX_ENSURE_MEM(entry);
	entry->nbor[next] = v->nbor[next];
	v->nbor[next] = entry;
	return nbor[next];
}

/**
 * @fn static inline struct Vertegs* vx_remlist(struct Vertegs** listp, int pos)
 *
 * @brief Remove, at a given position, an entry from a list.
 *
 * @param[in] listp The pointer to the head of the list.
 * @param[in] pos The position.
 *
 * @return The removed entry.
 */
static inline struct Vertegs* vx_remlist(struct Vertegs** listp, int pos)
{
	const size_t next = VX_LIST_NEXT;
	struct Vertegs* ret = NULL;
	struct Vertegs* v = vx_walk(vx_4nbor(listp), next, pos);

	ret = v->nbor[next];
	v->nbor[next] = ret->nbor[next];
	return ret;
}

/**
 * @fn static inline struct Vertegs* vx_inscirq(struct Vertegs* cirq, struct Vertegs* entry, int pos)
 *
 * @brief Insert, at a given position, an entry into a *cirq*.
 *
 * @param[in] cirq The head of the *cirq*.
 * @param[in] entry The new entry.
 * @param[in] pos The position.
 *
 * @return The new head.
 */
static inline struct Vertegs*
vx_inscirq(struct Vertegs* cirq, struct Vertegs* entry, int pos)
{
	const size_t next = VX_LIST_NEXT;
	const size_t prev = VX_LIST_PREV;
	struct Vertegs* v = NULL;

	VX_ENSURE_MEM(entry);
	if (!cirq) {
		entry->nbor[next] = entry;
		entry->nbor[prev] = entry;
		return entry;
	}

	if (pos > 0)
		v = vx_walk(cirq, next, pos);
	else if (pos < -1)
		v = vx_walk(cirq, prev, -(pos + 1));
	else
		v = cirq;
	entry->nbor[next] = v;
	entry->nbor[prev] = v->nbor[prev];
	v->nbor[prev] = entry;
	entry->nbor[prev]->nbor[next] = entry;
	return pos ? cirq : entry;
}

/**
 * @fn static inline struct Vertegs* vx_remcirq(struct Vertegs** cirqp, int pos)
 *
 * @brief Remove, at a given position, an entry from a *cirq*.
 *
 * @param[in] cirqp The pointer to the head of the *cirq*.
 * @param[in] pos The position.
 *
 * @return The removed entry.
 */
static inline struct Vertegs* vx_remcirq(struct Vertegs** cirqp, int pos)
{
	const size_t next = VX_LIST_NEXT;
	const size_t prev = VX_LIST_PREV;
	struct Vertegs* ret = NULL;

	VX_ENSURE_MEM(cirqp);
	ret = *cirqp;
	if (pos > 0)
		ret = vx_walk(ret, next, pos);
	else if (pos < 0)
		ret = vx_walk(ret, prev, -pos);
	ret->nbor[next]->nbor[prev] = ret->nbor[prev];
	ret->nbor[prev]->nbor[next] = ret->nbor[next];
	if (ret == ret->nbor[next])
		*cirqp = NULL;
	else if (ret == *cirqp)
		*cirqp = ret->nbor[next];
	return ret;
}

/**
 * @fn static inline struct Vertegs* vx_nextit(struct Vertegs*** it, size_t edge)
 *
 * @brief On a given edge, get the next entry from a graph iterator.
 *
 * @param[in,out] it The iterator.
 * @param[in] edge The edge index.
 *
 * @return The next entry.
 */
static inline struct Vertegs* vx_nextit(struct Vertegs*** it, size_t edge)
{
	struct Vertegs* ret = **it;

	VX_ENSURE_MEM(ret);
	*it = &ret->nbor[edge];
	return ret;
}

/**
 * @fn static inline enum VxRbColor vx_getrbcolor(struct Vertegs* v)
 *
 * @brief Get the color trait of a node from a red-black tree.
 *
 * @param[in] v The node.
 *
 * @return The color trait.
 */
static inline enum VxRbColor vx_getrbcolor(struct Vertegs* v)
{
	if (v && (intptr_t)v->nbor[VX_RB_PARCOL] & VX_TRAIT_MASK)
		return VX_RB_RED;
	return VX_RB_BLACK;
}

/**
 * @fn static inline struct Vertegs* vx_inorderfirst(struct Vertegs* v)
 *
 * @brief Get the first node of an in-order tree traversal.
 *
 * @param[in] v The node from the tree.
 *
 * @return The first node.
 */
static inline struct Vertegs* vx_inorderfirst(struct Vertegs* v)
{
	struct Vertegs* p = NULL;

	VX_ENSURE_MEM(v);
	for (;;) {
		if (v->nbor[VX_RB_LEFT]) {
			v = v->nbor[VX_RB_LEFT];
		} else {
			p = vx_unmark(v->nbor[VX_RB_PARCOL]);
			if (p && (p->nbor[VX_RB_RIGHT] == v))
				v = p;
			else
				break;
		}
	}
	return v;
}

/**
 * @fn static inline struct Vertegs* vx_inordernext(struct Vertegs* v)
 *
 * @brief Get the next node of an in-order tree traversal.
 *
 * @param[in] v The current node.
 *
 * @return The next node.
 */
static inline struct Vertegs* vx_inordernext(struct Vertegs* v)
{
	const size_t left = VX_RB_LEFT;
	const size_t right = VX_RB_RIGHT;
	struct Vertegs* p = NULL;

	VX_ENSURE_MEM(v);
	if (v->nbor[right])
		p = vx_walk(v->nbor[right], left, -1);
	else
		while ((p = vx_unmark(v->nbor[VX_RB_PARCOL])) &&
		       v == p->nbor[right])
			v = p;
	return p;
}

/**
 * @fn static inline struct Vertegs* vx_preordernext(struct Vertegs* v)
 *
 * @brief Get the next node of a pre-order tree traversal.
 *
 * @param[in] v The current node.
 *
 * @return The next node.
 */
static inline struct Vertegs* vx_preordernext(struct Vertegs* v)
{
	const size_t left = VX_RB_LEFT;
	const size_t right = VX_RB_RIGHT;
	struct Vertegs* p = NULL;

	VX_ENSURE_MEM(v);
	if (v->nbor[left])
		return v->nbor[left];

	if (v->nbor[right])
		return v->nbor[right];

	while ((p = vx_unmark(v->nbor[VX_RB_PARCOL]))) {
		if (v != p->nbor[right] && p->nbor[right])
			break;
		v = p;
	}
	return p ? p->nbor[right] : NULL;
}

/**
 * @fn static inline struct Vertegs* vx_postorderfirst(struct Vertegs* v)
 *
 * @brief Get the first node of a post-order tree traversal.
 *
 * @param[in] v The node from the tree.
 *
 * @return The first node.
 */
static inline struct Vertegs* vx_postorderfirst(struct Vertegs* v)
{
	VX_ENSURE_MEM(v);
	v = vx_walksafe(v, VX_RB_PARCOL, -1);
	for (;;) {
		if (v->nbor[VX_RB_LEFT])
			v = v->nbor[VX_RB_LEFT];
		else if (v->nbor[VX_RB_RIGHT])
			v = v->nbor[VX_RB_RIGHT];
		else
			break;
	}
	return v;
}

/**
 * @fn static inline struct Vertegs* vx_linkrb(struct Vertegs* node, struct Vertegs* parent)
 *
 * @brief Prepare a node for insertion in a red-black tree.
 *
 * @param[in,out] node The node prepared for insertion.
 * @param[in] parent The parent node.
 *
 * This will paint @a node red and set its parent pointer to @a parent. This
 * will also ensure that the left and right pointers of @a node are NULL. After
 * preparing the node for insertion, insert it with vx_insrebalrb().
 *
 * @see vx_insrebalrb().
 *
 * @return The prepared node.
 */
static inline struct Vertegs*
vx_linkrb(struct Vertegs* node, struct Vertegs* parent)
{
	VX_ENSURE_MEM(node);
	node->nbor[VX_RB_PARCOL] = parent;
	node->nbor[VX_RB_PARCOL] = vx_mark(node->nbor[VX_RB_PARCOL], VX_RB_RED);
	node->nbor[VX_RB_LEFT] = NULL;
	node->nbor[VX_RB_RIGHT] = NULL;
	return node;
}

/**
 * @fn static inline void vx_unlinkrb(struct Vertegs* v)
 *
 * @brief In a red-black tree set to null the pointer that stores the address of the given node.
 *
 * @param[in] v The given node.
 */
static inline void vx_unlinkrb(struct Vertegs* v)
{
	struct Vertegs* p = vx_unmark(v->nbor[VX_RB_PARCOL]);

	if (p) {
		if (v == p->nbor[VX_RB_LEFT])
			p->nbor[VX_RB_LEFT] = NULL;
		else
			p->nbor[VX_RB_RIGHT] = NULL;
	}
}

/* @cond */
static inline struct Vertegs*
vx_rotleftrb(struct Vertegs* v, struct Vertegs* root)
{
	struct Vertegs* p = vx_unmark(v->nbor[VX_RB_PARCOL]);
	struct Vertegs* y = v->nbor[VX_RB_RIGHT];
	struct Vertegs** tmp = NULL;

	VX_ENSURE_MEM(y);
	tmp = &y->nbor[VX_RB_LEFT];
	v->nbor[VX_RB_RIGHT] = *tmp;
	if (*tmp) {
		tmp = &(*tmp)->nbor[VX_RB_PARCOL];
		*tmp = vx_mark(v, (intptr_t)*tmp);
	}

	tmp = &y->nbor[VX_RB_PARCOL];
	*tmp = vx_mark(p, (intptr_t)*tmp);
	if (!p)
		root = y;
	else if (v == p->nbor[VX_RB_LEFT])
		p->nbor[VX_RB_LEFT] = y;
	else
		p->nbor[VX_RB_RIGHT] = y;

	y->nbor[VX_RB_LEFT] = v;
	tmp = &v->nbor[VX_RB_PARCOL];
	*tmp = vx_mark(y, (intptr_t)*tmp);
	return root;
}

static inline struct Vertegs*
vx_rotrightrb(struct Vertegs* v, struct Vertegs* root)
{
	struct Vertegs* p = vx_unmark(v->nbor[VX_RB_PARCOL]);
	struct Vertegs* y = v->nbor[VX_RB_LEFT];
	struct Vertegs** tmp = NULL;

	VX_ENSURE_MEM(y);
	tmp = &y->nbor[VX_RB_RIGHT];
	v->nbor[VX_RB_LEFT] = *tmp;
	if (*tmp) {
		tmp = &(*tmp)->nbor[VX_RB_PARCOL];
		*tmp = vx_mark(v, (intptr_t)*tmp);
	}

	tmp = &y->nbor[VX_RB_PARCOL];
	*tmp = vx_mark(p, (intptr_t)*tmp);
	if (!p)
		root = y;
	else if (v == p->nbor[VX_RB_LEFT])
		p->nbor[VX_RB_LEFT] = y;
	else
		p->nbor[VX_RB_RIGHT] = y;

	y->nbor[VX_RB_RIGHT] = v;
	tmp = &v->nbor[VX_RB_PARCOL];
	*tmp = vx_mark(y, (intptr_t)*tmp);
	return root;
}

static inline void vx_swaprbcolors(struct Vertegs* a, struct Vertegs* b)
{
	const enum VxRbColor tmp = vx_getrbcolor(a);

	a->nbor[VX_RB_PARCOL] =
		vx_mark(a->nbor[VX_RB_PARCOL], vx_getrbcolor(b));
	b->nbor[VX_RB_PARCOL] = vx_mark(b->nbor[VX_RB_PARCOL], tmp);
}
/* @endcond */

/**
 * @fn static inline struct Vertegs* vx_insrebalrb(struct Vertegs* root, struct Vertegs* v)
 *
 * @brief Insert a node in a red-black tree and rebalance the tree.
 *
 * @param[in,out] root The root of the red-black tree.
 * @param[in,out] v The inserted node.
 *
 * @return The new root.
 *
 * @note Prepare @a node with vx_linkrb() before insertion.
 *
 * @see vx_linkrb().
 */
static inline struct Vertegs*
vx_insrebalrb(struct Vertegs* root, struct Vertegs* v)
{
	VX_ENSURE_MEM(v);
	for (struct Vertegs *p = NULL, *g = NULL, *u = NULL;;) {
		p = vx_unmark(v->nbor[VX_RB_PARCOL]);
		if (vx_getrbcolor(p) == VX_RB_BLACK) {
			/* Case 0: done. */
			if (!p) {
				v->nbor[VX_RB_PARCOL] = vx_mark(
					v->nbor[VX_RB_PARCOL], VX_RB_BLACK);
				root = v;
			}
			break;
		}

		/*
		 * P is red and, as only black nodes can be nil or root, thus:
		 *     a) it cannot be nil,
		 *     b) its parent (G) cannot be nil.
		 */
		g = vx_unmark(p->nbor[VX_RB_PARCOL]);
		u = (p == g->nbor[VX_RB_LEFT]) ? g->nbor[VX_RB_RIGHT]
		                               : g->nbor[VX_RB_LEFT];
		if (vx_getrbcolor(u) == VX_RB_RED) {
			/* Case 1: recolor. */
			p->nbor[VX_RB_PARCOL] =
				vx_mark(p->nbor[VX_RB_PARCOL], VX_RB_BLACK);
			u->nbor[VX_RB_PARCOL] =
				vx_mark(u->nbor[VX_RB_PARCOL], VX_RB_BLACK);
			g->nbor[VX_RB_PARCOL] =
				vx_mark(g->nbor[VX_RB_PARCOL], VX_RB_RED);
			v = g;
			continue;
		}

		/*
		 * Case 2: rotate and recolor.
		 * Go to step 2.0 or 2.1?
		 */
		if (p == g->nbor[VX_RB_LEFT]) {
			/*
			 * Step 2.0: left.
			 * Is N inner grandchild of G?
			 */
			if (v == p->nbor[VX_RB_RIGHT]) {
				/*
				 * N is inner grandchild of G.
				 * Step 2.0.0: left-right.
				 */
				root = vx_rotleftrb(p, root);
				v = p;
				p = vx_unmark(v->nbor[VX_RB_PARCOL]);
				/* Go to step 2.0.1. */
			}
			/* Step 2.0.1: left-left. */
			root = vx_rotrightrb(g, root);
			/* Go to step 2.2. */
		} else {
			/*
			 * Step 2.1: right.
			 * Is N inner grandchild of G?
			 */
			if (v == p->nbor[VX_RB_LEFT]) {
				/*
				 * N is inner grandchild of G.
				 * Step 2.1.0: right-left.
				 */
				root = vx_rotrightrb(p, root);
				v = p;
				p = vx_unmark(v->nbor[VX_RB_PARCOL]);
				/* Go to step 2.1.1. */
			}
			/* Step 2.1.1: right-right. */
			root = vx_rotleftrb(g, root);
			/* Go to step 2.2. */
		}

		/* Step 2.2: swap colors. */
		vx_swaprbcolors(p, g);
		v = p;
	}
	VX_ASSERT(root);
	return root;
}

#endif /* VERTEGS_VERTEX_H */
