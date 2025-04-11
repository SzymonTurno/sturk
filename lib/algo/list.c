#include "ub/list.h"

struct UBunnode** ub_unnode_hand(struct UBunnode** nodep, int pos)
{
	UB_ENSURE(nodep, "Null pointer.");
	for (; *nodep && pos--; nodep = &(*nodep)->next)
		;
	return nodep;
}

struct UBunnode* ub_unnode_ins(struct UBunnode* head, struct UBunnode* node,
	int pos)
{
	struct UBunnode** i = ub_list_hand(&head, pos);

	UB_ENSURE(node, "Null pointer.");
	node->next = *i;
	*i = node;
	return head;
}

struct UBunnode* ub_unnode_rem(struct UBunnode** headp, int pos)
{
	struct UBunnode** i = ub_list_hand(headp, pos);
	struct UBunnode* ret = *i;

	*i = (*i)->next;
	return ret;
}
