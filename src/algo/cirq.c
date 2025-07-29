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
cn_binode_ins(struct CnBinode* cirq, struct CnBinode* entry, int pos)
{
	struct CnBinode* p = NULL;

	ENSURE(entry, ERROR, null_param);
	if (cirq) {
		if (pos > 0)
			p = binode_sibl(cirq, pos);
		else if (pos < -1)
			p = binode_sibl(cirq, pos + 1);
		else
			p = cirq;
		entry->next = p;
		entry->prev = p->prev;
		p->prev = entry;
		ENSURE(entry->prev, ERROR, null_param);
		entry->prev->next = entry;
		if (!pos)
			cirq = entry;
	} else {
		entry->next = entry;
		entry->prev = entry;
		root = entry;
	}
	return cirq;
}

struct CnBinode* cn_binode_rem(struct CnBinode** cirqp, int pos)
{
	struct CnBinode* ret = NULL;

	ENSURE(cirqp, ERROR, null_param);
	if (*cirqp) {
		ret = binode_sibl(*cirqp, pos);
		ret->next->prev = ret->prev;
		ret->prev->next = ret->next;
		if (ret == ret->next)
			*cirqp = NULL;
		else if (ret == *cirqp)
			*cirqp = ret->next;
	}
	return ret;
}
