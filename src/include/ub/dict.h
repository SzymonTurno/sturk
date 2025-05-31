#ifndef UB_DICT_H
#define UB_DICT_H

#include "ub/rbtree.h"
#include "ub/arith.h"
#include "ub/logger/except.h"

struct UBstrnode {
	struct UBrbnode node;
	char* str;
};

#define UB_DICT(name, type)                                                   \
	name { struct UBstrnode strnode; type data; }

struct UBstrnode*
ub_strnode_ins(struct UBstrnode* root, struct UBstrnode* node);

struct UBstrnode* ub_strnode_find(struct UBstrnode* root, const char* str);

static inline struct UBstrnode* ub_strnode_from(struct UBrbnode* ptr)
{
	return ub_container_of(ptr, struct UBstrnode, node);
}

#ifdef __STRICT_ANSI__
#define ub_dict_cast(dict) ((struct UBstrnode*)(dict))

#define ub_dict_setk(dict, key) ((dict)->strnode.str = (key))

#define ub_dict_getk(dict) ((dict)->strnode.str)

#define ub_dict_data(dict) (&(dict)->data)
#else /* __STRICT_ANSI__ */
#define ub_dict_cast(dict)                                                    \
	({                                                                     \
		__typeof__(dict) _dict = (dict);                               \
									       \
		_dict ? &_dict->strnode : NULL;                                \
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
#endif /* __STRICT_ANSI__ */

#ifdef __STRICT_ANSI__
#define ub_dict_ins(root, node)                                               \
	((void*)ub_strnode_ins(ub_dict_cast(root), ub_dict_cast(node)))

#define ub_dict_find(root, key)                                               \
	((void*)ub_strnode_find(ub_dict_cast(root), (key)))

#define ub_dict_leftmost(root)                                                \
	((void*)(ub_strnode_from(ub_rb_leftmost(&dict_cast(dict)->node, 0)))

#define ub_dict_next(root)                                                    \
	((void*)(ub_strnode_from(ub_rb_next(&dict_cast(dict)->node, 0)))
#else /* __STRICT_ANSI__ */
#define ub_dict_ins(root, node)                                               \
	({                                                                     \
		__typeof__(root) _root = (root);                               \
									       \
		ub_container_of(                                               \
			ub_strnode_ins(ub_dict_cast(_root),                    \
				ub_dict_cast(node)),                           \
			__typeof__(*root),                                     \
			strnode);                                              \
	})

#define ub_dict_find(root, key)                                               \
	({                                                                     \
		__typeof__(root) _root = (root);                               \
									       \
		ub_container_of(                                               \
			ub_strnode_find(ub_dict_cast(_root), (key)),           \
			__typeof__(*root),                                     \
			strnode);                                              \
	})

#define ub_dict_leftmost(root)                                                \
	({                                                                     \
		__typeof__(root) _root = (root);                               \
									       \
		ub_container_of(                                               \
			ub_strnode_from(                                       \
				ub_rb_leftmost(&dict_cast(_root)->node)),      \
			__typeof__(*root),                                     \
			strnode);                                              \
	})

#define ub_dict_next(root)                                                    \
	({                                                                     \
		__typeof__(root) _root = (root);                               \
									       \
		ub_container_of(                                               \
			ub_strnode_from(                                       \
				ub_rb_next(&dict_cast(_root)->node, 0)),       \
			__typeof__(*root),                                     \
			strnode);                                              \
	})
#endif /* __STRICT_ANSI__ */
#endif /* UB_DICT_H */
