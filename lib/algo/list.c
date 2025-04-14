#include "UB/list.h"
#include "UB/logger.h"

struct UBunnode** ub_unnode_hand(struct UBunnode** nodep, int pos)
{
	ENSURE(nodep, "Null pointer.");
	for (; *nodep && pos--; nodep = &(*nodep)->next)
		;
	return nodep;
}

struct UBunnode* ub_unnode_ins(struct UBunnode* head, struct UBunnode* node,
	int pos)
{
	struct UBunnode** i = list_hand(&head, pos);

	ENSURE(node, "Null pointer.");
	node->next = *i;
	*i = node;
	return head;
}

struct UBunnode* ub_unnode_rem(struct UBunnode** headp, int pos)
{
	struct UBunnode** i = list_hand(headp, pos);
	struct UBunnode* ret = *i;

	*i = (*i)->next;
	return ret;
}
