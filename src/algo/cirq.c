#include "cantil/cirq.h"
#include "cantil/logger/except.h"
#include "cantil/logger/trace.h"

struct CnBinode* cn_binode_sibl(struct CnBinode* node, int pos)
{
	while (pos > 0) {
		ENSURE(node, ERROR, null_param);
		node = node->next;
		--pos;
	}

	while (pos < 0) {
		ENSURE(node, ERROR, null_param);
		node = node->prev;
		++pos;
	}
	return node;
}

struct CnBinode*
cn_binode_ins(struct CnBinode* root, struct CnBinode* entry, int pos)
{
	struct CnBinode* p = NULL;

	ENSURE(entry, ERROR, null_param);
	if (root) {
		if (pos > 0)
			p = binode_sibl(root, pos);
		else if (pos < -1)
			p = binode_sibl(root, pos + 1);
		else
			p = root;
		entry->next = p;
		entry->prev = p->prev;
		p->prev = entry;
		ENSURE(entry->prev, ERROR, null_param);
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

	ENSURE(nodep, ERROR, null_param);
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
