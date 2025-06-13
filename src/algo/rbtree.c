#include "cantil/rbtree.h"
#include "cantil/bits.h"
#include "cantil/logger/except.h"
#include <stddef.h>

#define COLOR_MASK ((intptr_t)BIT(0))

#define MODULE_ENSURE_MEM(ptr)                                                 \
	do {                                                                   \
		if ((ptr) == NULL) {                                           \
			RAISE(EXCEPT.NULL_PARAM);                              \
			return NULL;                                           \
		}                                                              \
	} while (0)

static void paint_red(struct CnRbnode* node)
{
	node->u.parcol |= COLOR_MASK;
}

static void paint_black(struct CnRbnode* node)
{
	node->u.parcol &= ~COLOR_MASK;
}

static int painted_red(struct CnRbnode* node)
{
	return (node && (node->u.parcol & COLOR_MASK));
}

static void set_parent(struct CnRbnode* node, struct CnRbnode* parent)
{
	node->u.parcol = ((intptr_t)parent) | (node->u.parcol & COLOR_MASK);
}

static struct CnRbnode* get_parent(struct CnRbnode* node)
{
	return (struct CnRbnode*)(node->u.parcol & ~COLOR_MASK);
}

static struct CnRbnode* rot_left(struct CnRbnode* node, struct CnRbnode* root)
{
	struct CnRbnode* p = get_parent(node);
	struct CnRbnode* y = node->right;

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

static struct CnRbnode* rot_right(struct CnRbnode* node, struct CnRbnode* root)
{
	struct CnRbnode* p = get_parent(node);
	struct CnRbnode* y = node->left;

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

static struct CnRbnode* get_inordersucc(struct CnRbnode* node)
{
	struct CnRbnode* p = NULL;

	if (node->right) {
		p = rb_leftmost(node->right);
	} else {
		while ((p = get_parent(node)) && node == p->right)
			node = p;
	}
	return p;
}

static struct CnRbnode* get_preordersucc(struct CnRbnode* node)
{
	struct CnRbnode* p = NULL;

	if (node->left)
		return node->left;

	if (node->right)
		return node->right;

	while ((p = get_parent(node)) && (node == p->right || !p->right))
		node = p;
	return p ? p->right : NULL;
}

static struct CnRbnode* get_postordersucc(struct CnRbnode* node)
{
	(void)node;
	RAISE(EXCEPT.NOT_SUPPORTED);
	return NULL;
}

struct CnRbnode* cn_rb_link(struct CnRbnode* node, struct CnRbnode* parent)
{
	MODULE_ENSURE_MEM(node);
	paint_red(node);
	set_parent(node, parent);
	node->left = NULL;
	node->right = NULL;
	return node;
}

struct CnRbnode* cn_rb_insrebal(struct CnRbnode* root, struct CnRbnode* node)
{
	MODULE_ENSURE_MEM(node);
	for (struct CnRbnode *p = NULL, *g = NULL, *u = NULL;;) {
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

struct CnRbnode* cn_rb_parent(struct CnRbnode* node)
{
	MODULE_ENSURE_MEM(node);
	return get_parent(node);
}

struct CnRbnode* cn_rb_deepest(struct CnRbnode* node)
{
	MODULE_ENSURE_MEM(node);
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

struct CnRbnode* cn_rb_leftmost(struct CnRbnode* node)
{
	MODULE_ENSURE_MEM(node);
	while (node->left)
		node = node->left;
	return node;
}

struct CnRbnode* cn_rb_next(struct CnRbnode* node, enum CnBstTrav trav)
{
	MODULE_ENSURE_MEM(node);
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
