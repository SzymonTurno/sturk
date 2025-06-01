#include "UB/rbtree.h"
#include "ub/bits.h"
#include "UB/logger/except.h"
#include <stddef.h>

#define COLOR_MASK ((intptr_t)UBit(0))

#define RBNODE_ENSURE(node)                                                   \
	do {                                                                   \
		if (!node) {                                                   \
			RAISE(ECODES.null_param);                              \
			return NULL;                                           \
		}                                                              \
	} while (0)

static void paint_red(struct UBrbnode* node)
{
	node->u.parcol |= COLOR_MASK;
}

static void paint_black(struct UBrbnode* node)
{
	node->u.parcol &= ~COLOR_MASK;
}

static int painted_red(struct UBrbnode* node)
{
	return (node && (node->u.parcol & COLOR_MASK));
}

static void set_parent(struct UBrbnode* node, struct UBrbnode* parent)
{
	node->u.parcol = ((intptr_t)parent) | (node->u.parcol & COLOR_MASK);
}

static struct UBrbnode* get_parent(struct UBrbnode* node)
{
	return (struct UBrbnode*)(node->u.parcol & ~COLOR_MASK);
}

static struct UBrbnode* rot_left(struct UBrbnode* node, struct UBrbnode* root)
{
	struct UBrbnode* p = get_parent(node);
	struct UBrbnode* y = node->right;

	node->right = y->left;
	if (node->right)
		set_parent(node->right, node);

	set_parent(y, p);
	if (!p)
		root = y;
	else if (node == p->left)
		p->left = y;
	else
		p->right = y;

	y->left = node;
	set_parent(node, y);
	return root;
}

static struct UBrbnode* rot_right(struct UBrbnode* node, struct UBrbnode* root)
{
	struct UBrbnode* p = get_parent(node);
	struct UBrbnode* y = node->left;

	node->left = y->right;
	if (node->left)
		set_parent(node->left, node);

	set_parent(y, p);
	if (!p)
		root = y;
	else if (node == p->left)
		p->left = y;
	else
		p->right = y;

	y->right = node;
	set_parent(node, y);
	return root;
}

static struct UBrbnode* get_inordersucc(struct UBrbnode* node)
{
	struct UBrbnode* p = NULL;

	if (node->right) {
		p = rb_leftmost(node->right);
	} else {
		while ((p = get_parent(node)) && node == p->right)
			node = p;
	}
	return p;
}

static struct UBrbnode* get_preordersucc(struct UBrbnode* node)
{
	struct UBrbnode* p = NULL;

	if (node->left)
		return node->left;

	if (node->right)
		return node->right;

	while ((p = get_parent(node)) && (node == p->right || !p->right))
		node = p;
	return p ? p->right : NULL;
}

static struct UBrbnode* get_postordersucc(struct UBrbnode* node)
{
	(void) node;
	RAISE(ECODES.not_supported);
	return NULL;
}

struct UBrbnode* ub_rb_link(struct UBrbnode* node, struct UBrbnode* parent)
{
	RBNODE_ENSURE(node);
	paint_red(node);
	set_parent(node, parent);
	node->left = NULL;
	node->right = NULL;
	return node;
}

struct UBrbnode* ub_rb_insrebal(struct UBrbnode* root, struct UBrbnode* node)
{
	RBNODE_ENSURE(node);
	for (struct UBrbnode *p = NULL, *g = NULL, *u = NULL;;) {
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
		u = (p == g->left) ? g->right : g->left;
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
		if (p == g->left) {
			/*
			 * Step 2.0: left.
			 * Is N inner grandchild of G?
			 */
			if (node == p->right) {
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
			if (node == p->left) {
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
	return root;
}

struct UBrbnode* ub_rb_parent(struct UBrbnode* node)
{
	RBNODE_ENSURE(node);
	return get_parent(node);
}

struct UBrbnode* ub_rb_deepest(struct UBrbnode* node)
{
	RBNODE_ENSURE(node);
	for (;;) {
		if (node->left)
			node = node->left;
		else if (node->right)
			node = node->right;
		else
			break;
	}
	return node;
}

struct UBrbnode* ub_rb_leftmost(struct UBrbnode* node)
{
	RBNODE_ENSURE(node);
	while (node->left)
		node = node->left;
	return node;
}

struct UBrbnode* ub_rb_next(struct UBrbnode* node, enum CnBstTrav trav)
{
	RBNODE_ENSURE(node);
	switch (trav) {
	case CN_BST_TRAV_PREORDER:
		return get_preordersucc(node);
	case CN_BST_TRAV_POSTORDER:
		return get_postordersucc(node);
	default:
		break;
	}
	return get_inordersucc(node);
}
