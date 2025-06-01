#include "cantil/cirq.h"
#include "cantil/logger/except.h"

struct CnBinode* cn_binode_sibl(struct CnBinode* node, int pos)
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

struct CnBinode* cn_binode_ins(struct CnBinode* root, struct CnBinode* entry,
	int pos)
{
	ENSURE(entry, ECODES.null_param);
	if (root) {
		if (pos > 0)
			root = binode_sibl(root, pos);
		else if (pos < -1)
			root = binode_sibl(root, pos + 1);
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

struct CnBinode* cn_binode_rem(struct CnBinode** nodep, int pos)
{
	struct CnBinode* ret = NULL;

	ENSURE(nodep, ECODES.null_param);
	if (*nodep) {
		ret = binode_sibl(*nodep, pos);
		ret->next->prev = ret->prev;
		ret->prev->next = ret->next;
		if (ret == ret->next)
			*nodep = NULL;
		else if (ret == *nodep)
			*nodep = ret->next;
	}
	return ret;
}
