#include "cantil/list.h"
#include "cantil/logger/except.h"

struct CnUnnode** cn_unnode_hand(struct CnUnnode** nodep, int pos)
{
	ENSURE(nodep, ECODES.null_param);
	for (; *nodep && pos--; nodep = &(*nodep)->next)
		;
	return nodep;
}

struct CnUnnode* cn_unnode_ins(struct CnUnnode* head, struct CnUnnode* node,
	int pos)
{
	struct CnUnnode** i = list_hand(&head, pos);

	ENSURE(node, ECODES.null_param);
	node->next = *i;
	*i = node;
	return head;
}

struct CnUnnode* cn_unnode_rem(struct CnUnnode** headp, int pos)
{
	struct CnUnnode** i = list_hand(headp, pos);
	struct CnUnnode* ret = *i;

	*i = (*i)->next;
	return ret;
}
