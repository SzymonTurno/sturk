#ifndef CN_DICT_H
#define CN_DICT_H

#include "cn/arith.h"
#include "cn/logger/except.h"
#include "cn/rbtree.h"

struct CnStrnode {
	struct CnRbnode node;
	char* str;
};

#define CN_DICT(name, type)                                                    \
	name                                                                   \
	{                                                                      \
		struct CnStrnode strnode;                                      \
		type data;                                                     \
	}

struct CnStrnode*
cn_strnode_ins(struct CnStrnode* root, struct CnStrnode* node);

struct CnStrnode* cn_strnode_find(struct CnStrnode* root, const char* str);

static inline struct CnStrnode* cn_strnode_from(struct CnRbnode* ptr)
{
	return cn_container_of(ptr, struct CnStrnode, node);
}

#ifdef __STRICT_ANSI__

#define cn_dict_cast(dict) ((struct CnStrnode*)(dict))

#define cn_dict_setk(dict, key) ((dict)->strnode.str = (key))

#define cn_dict_getk(dict) ((dict)->strnode.str)

#define cn_dict_data(dict) (&(dict)->data)

#else /* __STRICT_ANSI__ */

#define cn_dict_cast(dict)                                                     \
	({                                                                     \
		__typeof__(dict) _dict = (dict);                               \
                                                                               \
		_dict ? &_dict->strnode : NULL;                                \
	})

#define cn_dict_setk(dict, key)                                                \
	({                                                                     \
		__typeof__(dict) _dict = (dict);                               \
                                                                               \
		CN_ENSURE(_dict, ERROR, null_param);                           \
		_dict->strnode.str = (key);                                    \
	})

#define cn_dict_getk(dict)                                                     \
	({                                                                     \
		__typeof__(dict) _dict = (dict);                               \
                                                                               \
		CN_ENSURE(_dict, ERROR, null_param);                           \
		_dict->strnode.str;                                            \
	})

#define cn_dict_data(dict)                                                     \
	({                                                                     \
		__typeof__(dict) _dict = (dict);                               \
                                                                               \
		CN_ENSURE(_dict, ERROR, null_param);                           \
		&_dict->data;                                                  \
	})

#endif /* __STRICT_ANSI__ */

#ifdef __STRICT_ANSI__

#define cn_dict_ins(root, node)                                                \
	((void*)cn_strnode_ins(cn_dict_cast(root), cn_dict_cast(node)))

#define cn_dict_find(root, key)                                                \
	((void*)cn_strnode_find(cn_dict_cast(root), (key)))

#define cn_dict_first(root)                                                    \
	((void*)(cn_strnode_from(cn_rb_smallest(&dict_cast(dict)->node, 0)))

#define cn_dict_next(root)                                                     \
	((void*)(cn_strnode_from(cn_rb_next(&dict_cast(dict)->node, 0)))

#else /* __STRICT_ANSI__ */

#define cn_dict_ins(root, node)                                                \
	({                                                                     \
		__typeof__(root) _root = (root);                               \
                                                                               \
		cn_container_of(                                               \
			cn_strnode_ins(                                        \
				cn_dict_cast(_root), cn_dict_cast(node)),      \
			__typeof__(*root), strnode);                           \
	})

#define cn_dict_find(root, key)                                                \
	({                                                                     \
		__typeof__(root) _root = (root);                               \
                                                                               \
		cn_container_of(                                               \
			cn_strnode_find(cn_dict_cast(_root), (key)),           \
			__typeof__(*root), strnode);                           \
	})

#define cn_dict_first(root)                                                    \
	({                                                                     \
		__typeof__(root) _root = (root);                               \
                                                                               \
		cn_container_of(                                               \
			cn_strnode_from(                                       \
				cn_rb_smallest(&dict_cast(_root)->node)),      \
			__typeof__(*root), strnode);                           \
	})

#define cn_dict_next(root)                                                     \
	({                                                                     \
		__typeof__(root) _root = (root);                               \
                                                                               \
		cn_container_of(                                               \
			cn_strnode_from(cn_rb_next(                            \
				&dict_cast(_root)->node,                       \
				CN_BST_TRAV_INORDER)),                         \
			__typeof__(*root), strnode);                           \
	})

#endif /* __STRICT_ANSI__ */

#endif /* CN_DICT_H */
