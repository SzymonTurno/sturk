/**
 * @file cn/dict.h
 * @brief Dictionary.
 *
 * This dictionary is a key-value data structure where the key is always a
 * sequence of characters (ie. string, char array).
 */

#ifndef CN_DICT_H
#define CN_DICT_H

#include "cn/arith.h"
#include "cn/logger/except.h"
#include "cn/rbtree.h"

#ifdef __STRICT_ANSI__

#define cn_dict_cast(dict) ((struct CnStrnode*)(dict))

#define cn_dict_setk(dict, key) ((dict)->strnode.str = (key))

#define cn_dict_getk(dict) ((dict)->strnode.str)

#define cn_dict_data(dict) (&(dict)->data)

#define cn_dict_ins(dict, node)                                                \
	((void*)cn_strnode_ins(cn_dict_cast(dict), cn_dict_cast(node)))

#define cn_dict_find(dict, key)                                                \
	((void*)cn_strnode_find(cn_dict_cast(dict), (key)))

#define cn_dict_first(dict)                                                    \
	((void*)(cn_strnode_from(cn_rb_smallest(&dict_cast(dict)->node, 0)))

#define cn_dict_next(dict)                                                     \
	((void*)(cn_strnode_from(cn_rb_next(&dict_cast(dict)->node, 0)))

#else /* not defined: __STRICT_ANSI__ */

/**
 * @def cn_dict_cast(dict)
 * @brief *** todo ***.
 * @param[in] dict Input.
 * @returns *** todo ***.
 *
 * No type check for @a dict with __STRICT_ANSI__ build.
 */
#define cn_dict_cast(dict)                                                     \
	({                                                                     \
		__typeof__(dict) _dict2 = (dict);                              \
                                                                               \
		_dict2 ? &_dict2->strnode : NULL;                              \
	})

/**
 * @def cn_dict_setk(dict, key)
 * @brief *** todo ***.
 * @param[in,out] dict Input/output.
 * @param[in] key Input.
 *
 * No type check for @a dict with __STRICT_ANSI__ build.
 */
#define cn_dict_setk(dict, key)                                                \
	({                                                                     \
		__typeof__(dict) _dict = (dict);                               \
                                                                               \
		CN_ENSURE(_dict, CN_ERROR, null_param);                        \
		_dict->strnode.str = (key);                                    \
	})

/**
 * @def cn_dict_getk(dict)
 * @brief *** todo ***.
 * @param[in] dict Input.
 * @returns *** todo ***.
 *
 * No type check for @a dict with __STRICT_ANSI__ build.
 */
#define cn_dict_getk(dict)                                                     \
	({                                                                     \
		__typeof__(dict) _dict = (dict);                               \
                                                                               \
		CN_ENSURE(_dict, CN_ERROR, null_param);                        \
		_dict->strnode.str;                                            \
	})

/**
 * @def cn_dict_data(dict)
 * @brief *** todo ***.
 * @param[in] dict Input.
 * @returns *** todo ***.
 *
 * No type check for @a dict with __STRICT_ANSI__ build.
 */
#define cn_dict_data(dict)                                                     \
	({                                                                     \
		__typeof__(dict) _dict = (dict);                               \
                                                                               \
		CN_ENSURE(_dict, CN_ERROR, null_param);                        \
		&_dict->data;                                                  \
	})

/**
 * @def cn_dict_ins(dict, node)
 * @brief *** todo ***.
 * @param[in,out] dict Input/output.
 * @param[in,out] node Input/output.
 * @returns *** todo ***.
 *
 * No type check for @a dict with __STRICT_ANSI__ build.
 */
#define cn_dict_ins(dict, node)                                                \
	({                                                                     \
		__typeof__(dict) _dict = (dict);                               \
                                                                               \
		cn_container_of(                                               \
			cn_strnode_ins(                                        \
				cn_dict_cast(_dict), cn_dict_cast(node)),      \
			__typeof__(*dict), strnode);                           \
	})

/**
 * @def cn_dict_find(dict, key)
 * @brief *** todo ***.
 * @param[in] dict Input.
 * @param[in] key Input.
 * @returns *** todo ***.
 *
 * No type check for @a dict with __STRICT_ANSI__ build.
 */
#define cn_dict_find(dict, key)                                                \
	({                                                                     \
		__typeof__(dict) _dict = (dict);                               \
                                                                               \
		cn_container_of(                                               \
			cn_strnode_find(cn_dict_cast(_dict), (key)),           \
			__typeof__(*dict), strnode);                           \
	})

/**
 * @def cn_dict_first(dict)
 * @brief *** todo ***.
 * @param[in] dict Input.
 * @returns *** todo ***.
 *
 * No type check for \a dict with __STRICT_ANSI__ build.
 */
#define cn_dict_first(dict)                                                    \
	({                                                                     \
		__typeof__(dict) _dict = (dict);                               \
                                                                               \
		cn_container_of(                                               \
			cn_strnode_from(                                       \
				cn_rb_smallest(&dict_cast(_dict)->node)),      \
			__typeof__(*dict), strnode);                           \
	})

/**
 * @def cn_dict_next(dict)
 * @brief *** todo ***.
 * @param[in] dict Input.
 * @returns *** todo ***.
 *
 * No type check for @a dict with __STRICT_ANSI__ build.
 */
#define cn_dict_next(dict)                                                     \
	({                                                                     \
		__typeof__(dict) _dict = (dict);                               \
                                                                               \
		cn_container_of(                                               \
			cn_strnode_from(cn_rb_next(                            \
				&dict_cast(_dict)->node,                       \
				CN_BST_TRAV_INORDER)),                         \
			__typeof__(*dict), strnode);                           \
	})

#endif /* __STRICT_ANSI__ */

/**
 * @def CN_DICT(name, type)
 * @brief *** todo ***.
 * @param[in] name Input.
 * @param[in] type Input.
 * @returns *** todo ***.
 */
#define CN_DICT(name, type)                                                    \
	name                                                                   \
	{                                                                      \
		struct CnStrnode strnode;                                      \
		type data;                                                     \
	}

/**
 * @struct CnStrnode
 * @brief *** todo ***.
 *
 * Members:
 * - node,
 * - str.
 */
struct CnStrnode {
	/**
	 * @var struct CnRbnode node
	 * @brief *** todo ***.
	 */
	struct CnRbnode node;

	/**
	 * @var char* str
	 * @brief *** todo ***.
	 */
	char* str;
};

/**
 * @fn struct CnStrnode* cn_strnode_ins(struct CnStrnode* root, struct CnStrnode* node)
 * @brief *** todo ***.
 * @param[in,out] root Input/output.
 * @param[in,out] node Input/output.
 * @returns *** todo ***.
 */
struct CnStrnode*
cn_strnode_ins(struct CnStrnode* root, struct CnStrnode* node);

/**
 * @fn struct CnStrnode* cn_strnode_find(struct CnStrnode* root, const char* str)
 * @brief *** todo ***.
 * @param[in,out] root Input/output.
 * @param[in] str Input.
 * @returns *** todo ***.
 */
struct CnStrnode* cn_strnode_find(struct CnStrnode* root, const char* str);

/**
 * @fn static inline struct CnStrnode* cn_strnode_from(struct CnRbnode* ptr)
 * @brief *** todo ***.
 * @param[in] ptr Input.
 * @returns *** todo ***.
 */
static inline struct CnStrnode* cn_strnode_from(struct CnRbnode* ptr)
{
	return cn_container_of(ptr, struct CnStrnode, node);
}

#endif /* CN_DICT_H */
