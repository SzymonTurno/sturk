#ifndef UB_DICT_H
#define UB_DICT_H

#include "ub/rbtree.h"
#include "ub/arith.h"
#include "ub/debug.h"

struct UBstrnode {
	char* str;
	struct UBrbnode node;
};

#define UB_DICT(name, type)                                                   \
	name { struct UBstrnode strnode; type data; }

struct UBstrnode*
ub_strnode_ins(struct UBstrnode* root, struct UBstrnode* node);

struct UBstrnode* ub_strnode_find(struct UBstrnode* root, const char* str);

#define ub_dict_cast(dict)                                                    \
	({                                                                     \
		__typeof__(dict) _dict = (dict);                               \
									       \
		ub_ensure(_dict, "Null pointer.");                             \
		&_dict->strnode;                                               \
	})

#define ub_dict_setk(dict, key)                                               \
	({                                                                     \
		__typeof__(dict) _dict = (dict);                               \
									       \
		ub_ensure(_dict, "Null pointer.");                             \
		_dict->strnode.str = (key);                                    \
	})

#define ub_dict_getk(dict)                                                    \
	({                                                                     \
		__typeof__(dict) _dict = (dict);                               \
									       \
		ub_ensure(_dict, "Null pointer.");                             \
		_dict->strnode.str;                                            \
	})

#define ub_dict_data(dict)                                                    \
	({                                                                     \
		__typeof__(dict) _dict = (dict);                               \
									       \
		ub_ensure(_dict, "Null pointer.");                             \
		&_dict->data;                                                  \
	})

#define ub_dict_cont(ptr, type) ub_container_of(ptr, type, strnode)

#define ub_dict_ins(root, node)                                               \
	({                                                                     \
		__typeof__(root) _root = (root);                               \
									       \
		ub_dict_cont(                                                  \
			ub_strnode_ins(                                        \
				_root ? &_root->strnode : NULL,                \
				&node->strnode),                               \
			__typeof__(*root));                                    \
	})

#define ub_dict_find(root, key)                                               \
	({                                                                     \
		__typeof__(root) _root = (root);                               \
									       \
		ub_dict_cont(                                                  \
			ub_strnode_find(_root ? &_root->strnode : NULL, (key)),\
			__typeof__(*root));                                    \
	})

#endif /* UB_DICT_H */
