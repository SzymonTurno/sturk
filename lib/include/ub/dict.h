#ifndef UB_DICT_H
#define UB_DICT_H

#include "ub/rbtree.h"
#include "ub/arith.h"
#include "ub/debug/except.h"

struct UBstrnode {
	struct UBrbnode node;
	char* str;
};

#define UB_DICT(name, type)                                                   \
	name { struct UBstrnode strnode; type data; }

struct UBstrnode*
ub_strnode_ins(struct UBstrnode* root, struct UBstrnode* node);

struct UBstrnode* ub_strnode_find(struct UBstrnode* root, const char* str);

#ifdef __STRICT_ANSI__
#define ub_dict_cast(dict) (&(dict)->strnode)

#define ub_dict_setk(dict, key) ((dict)->strnode.str = (key))

#define ub_dict_getk(dict) ((dict)->strnode.str)

#define ub_dict_data(dict) (&(dict)->data)

#define ub_dict_cont(ptr, type) ((type*)(ptr))
#else /* __STRICT_ANSI__ */
#define ub_dict_cast(dict)                                                    \
	({                                                                     \
		__typeof__(dict) _dict = (dict);                               \
									       \
		UB_ENSURE(_dict, UB_ECODES.null_param);                        \
		&_dict->strnode;                                               \
	})

#define ub_dict_setk(dict, key)                                               \
	({                                                                     \
		__typeof__(dict) _dict = (dict);                               \
									       \
		UB_ENSURE(_dict, UB_ECODES.null_param);                        \
		_dict->strnode.str = (key);                                    \
	})

#define ub_dict_getk(dict)                                                    \
	({                                                                     \
		__typeof__(dict) _dict = (dict);                               \
									       \
		UB_ENSURE(_dict, UB_ECODES.null_param);                        \
		_dict->strnode.str;                                            \
	})

#define ub_dict_data(dict)                                                    \
	({                                                                     \
		__typeof__(dict) _dict = (dict);                               \
									       \
		UB_ENSURE(_dict, UB_ECODES.null_param);                        \
		&_dict->data;                                                  \
	})

#define ub_dict_cont(ptr, type) ub_container_of(ptr, type, strnode)
#endif /* __STRICT_ANSI__ */

#ifdef __STRICT_ANSI__
#define ub_dict_ins(root, node)                                               \
	((void*)ub_strnode_ins((struct UBstrnode*)(root), &(node)->strnode))

#define ub_dict_find(root, key)                                               \
	((void*)ub_strnode_find((struct UBstrnode*)(root), (key)))
#else /* __STRICT_ANSI__ */
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
#endif /* __STRICT_ANSI__ */

#endif /* UB_DICT_H */
