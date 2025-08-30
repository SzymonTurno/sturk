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

#include "sturk/rbtree.h"
#include "sturk/bits.h"
#include "sturk/graph.h"
#include "sturk/logger/except.h"
#include "sturk/logger/trace.h"
#include <stddef.h>

#define COLOR_MASK ((intptr_t)BIT(0))

static void paint_red(struct StRbNode* node)
{
	graph_datap(node)->parcol |= COLOR_MASK;
}

static void paint_black(struct StRbNode* node)
{
	graph_datap(node)->parcol &= ~COLOR_MASK;
}

static int painted_red(struct StRbNode* node)
{
	return (node && (graph_datap(node)->parcol & COLOR_MASK));
}

static void set_parent(struct StRbNode* node, struct StRbNode* parent)
{
	graph_datap(node)->parcol =
		((intptr_t)parent) | (graph_datap(node)->parcol & COLOR_MASK);
}

static struct StRbNode* get_parent(struct StRbNode* node)
{
	return (struct StRbNode*)(graph_datap(node)->parcol & ~COLOR_MASK);
}

static void set_left(struct StRbNode* node, struct StRbNode* child)
{
	graph_2vx(node)->nbor[BST_LEFT] = graph_2vx(child);
}

static void set_right(struct StRbNode* node, struct StRbNode* child)
{
	graph_2vx(node)->nbor[BST_RIGHT] = graph_2vx(child);
}

static struct StRbNode* rot_left(struct StRbNode* node, struct StRbNode* root)
{
	struct StRbNode* p = get_parent(node);
	struct StRbNode* y = rb_right(node);

	set_right(node, rb_left(y));
	if (rb_right(node))
		set_parent(rb_right(node), node);

	set_parent(y, p);
	if (!p)
		root = y;
	else if (node == rb_left(p))
		set_left(p, y);
	else
		set_right(p, y);

	set_left(y, node);
	set_parent(node, y);
	return root;
}

static struct StRbNode* rot_right(struct StRbNode* node, struct StRbNode* root)
{
	struct StRbNode* p = get_parent(node);
	struct StRbNode* y = rb_left(node);

	set_left(node, rb_right(y));
	if (rb_left(node))
		set_parent(rb_left(node), node);

	set_parent(y, p);
	if (!p)
		root = y;
	else if (node == rb_left(p))
		set_left(p, y);
	else
		set_right(p, y);

	set_right(y, node);
	set_parent(node, y);
	return root;
}

static struct StRbNode* get_leftcorner(struct StRbNode* node)
{
	return graph_4vx(vx_walk(graph_2vx(node), BST_LEFT, -1), node);
}

static struct StRbNode* get_inorderfirst(struct StRbNode* node)
{
	struct StRbNode* p = NULL;

	ENSURE_MEM(node, ERROR);
	for (;;) {
		if (rb_left(node))
			node = rb_left(node);
		else {
			p = get_parent(node);
			if (p && (rb_right(p) == node))
				node = p;
			else
				break;
		}
	}
	return node;
}

static struct StRbNode* get_inordersucc(struct StRbNode* node)
{
	struct StRbNode* p = NULL;

	if (rb_right(node))
		p = get_leftcorner(rb_right(node));
	else
		while ((p = get_parent(node)) && node == rb_right(p))
			node = p;
	return p;
}

static struct StRbNode* get_preorderfirst(struct StRbNode* node)
{
	(void)node;
	RAISE(WARNING, not_supported);
	return NULL;
}

static struct StRbNode* get_preordersucc(struct StRbNode* node)
{
	struct StRbNode* p = NULL;

	if (rb_left(node))
		return rb_left(node);

	if (rb_right(node))
		return rb_right(node);

	while ((p = get_parent(node)) && (node == rb_right(p) || !rb_right(p)))
		node = p;
	return p ? rb_right(p) : NULL;
}

static struct StRbNode* get_postorderfirst(struct StRbNode* node)
{
	ENSURE_MEM(node, ERROR);
	while (get_parent(node))
		node = get_parent(node);

	for (;;) {
		if (rb_left(node))
			node = rb_left(node);
		else if (rb_right(node))
			node = rb_right(node);
		else
			break;
	}
	return node;
}

static struct StRbNode* get_postordersucc(struct StRbNode* node)
{
	(void)node;
	RAISE(WARNING, not_supported);
	return NULL;
}

struct StRbNode* st_rb_link(struct StRbNode* node, struct StRbNode* parent)
{
	ENSURE_MEM(node, ERROR);
	paint_red(node);
	set_parent(node, parent);
	set_left(node, NULL);
	set_right(node, NULL);
	return node;
}

struct StRbNode* st_rb_insrebal(struct StRbNode* root, struct StRbNode* node)
{
	ENSURE_MEM(node, ERROR);
	for (struct StRbNode *p = NULL, *g = NULL, *u = NULL;;) {
		p = get_parent(node);
		if (!painted_red(p)) {
			/* Case 0: done. */
			if (!p) {
				paint_black(node);
				root = node;
			}
			break;
		}

		/*
		 * P is red and, as only black nodes can be nil or root, thus:
		 *     a) it cannot be nil,
		 *     b) its parent (G) cannot be nil.
		 */
		g = get_parent(p);
		u = (p == rb_left(g)) ? rb_right(g) : rb_left(g);
		if (painted_red(u)) {
			/* Case 1: recolor. */
			paint_black(p);
			paint_black(u);
			paint_red(g);
			node = g;
			continue;
		}

		/*
		 * Case 2: rotate and recolor.
		 * Go to step 2.0 or 2.1?
		 */
		if (p == rb_left(g)) {
			/*
			 * Step 2.0: left.
			 * Is N inner grandchild of G?
			 */
			if (node == rb_right(p)) {
				/*
				 * N is inner grandchild of G.
				 * Step 2.0.0: left-right.
				 */
				root = rot_left(p, root);
				node = p;
				p = get_parent(node);
				/* Go to step 2.0.1. */
			}
			/* Step 2.0.1: left-left. */
			root = rot_right(g, root);
			/* Go to step 2.2. */
		} else {
			/*
			 * Step 2.1: right.
			 * Is N inner grandchild of G?
			 */
			if (node == rb_left(p)) {
				/*
				 * N is inner grandchild of G.
				 * Step 2.1.0: right-left.
				 */
				root = rot_right(p, root);
				node = p;
				p = get_parent(node);
				/* Go to step 2.1.1. */
			}
			/* Step 2.1.1: right-right. */
			root = rot_left(g, root);
			/* Go to step 2.2. */
		}
		/* Step 2.2: swap colors. */
		if (painted_red(p) && !painted_red(g)) {
			paint_black(p);
			paint_red(g);
		} else if (!painted_red(p) && painted_red(g)) {
			paint_red(p);
			paint_black(g);
		}
		node = p;
	}
	ENSURE(root, ERROR, sanity_fail);
	return root;
}

struct StRbNode* st_rb_parent(struct StRbNode* node)
{
	ENSURE_MEM(node, ERROR);
	return get_parent(node);
}

struct StRbNode* st_rb_first(struct StRbNode* node, enum StBstOrder order)
{
	ENSURE_MEM(node, ERROR);
	switch (order) {
	case ST_BST_PREORDER:
		return get_preorderfirst(node);
	case ST_BST_POSTORDER:
		return get_postorderfirst(node);
	default:
		break;
	}
	return get_inorderfirst(node);
}

struct StRbNode* st_rb_next(struct StRbNode* node, enum StBstOrder order)
{
	ENSURE_MEM(node, ERROR);
	switch (order) {
	case ST_BST_PREORDER:
		return get_preordersucc(node);
	case ST_BST_POSTORDER:
		return get_postordersucc(node);
	default:
		break;
	}
	return get_inordersucc(node);
}
