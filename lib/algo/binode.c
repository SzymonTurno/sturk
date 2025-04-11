#include "ub/cirq.h"

struct UBinode* ub_binode_sibl(struct UBinode* node, int pos)
{
	while (pos > 0 && node) {
		node = node->next;
		--pos;
	}

	while (pos < 0 && node) {
		node = node->prev;
		++pos;
	}
	return node;
}

struct UBinode* ub_binode_ins(struct UBinode* root, struct UBinode* entry,
	int pos)
{
	UB_ENSURE(entry, "Null pointer.");
	if (root) {
		if (pos > 0)
			root = ub_binode_sibl(root, pos);
		else if (pos < -1)
			root = ub_binode_sibl(root, pos + 1);
		entry->next = root;
		entry->prev = root->prev;
		root->prev = entry;
		entry->prev->next = entry;
		if (!pos)
			root = entry;
	} else {
		entry->next = entry;
		entry->prev = entry;
		root = entry;
	}
	return root;
}

struct UBinode* ub_binode_rem(struct UBinode** nodep, int pos)
{
	struct UBinode* ret = NULL;

	if (*nodep) {
		ret = ub_binode_sibl(*nodep, pos);
		ret->next->prev = ret->prev;
		ret->prev->next = ret->next;
		if (ret == ret->next)
			*nodep = NULL;
		else if (ret == *nodep)
			*nodep = ret->next;
	}
	return ret;
}
