/**
 * @file cn/dict.h
 *
 * @brief Dictionary.
 *
 * This header file provides data types, functions and macros that define and
 * operate on dictionaries. These dictionaries are key-value data structures
 * where the key is always a sequence of characters (ie. string, char array).
 *
 *
 * Features
 * --------
 *
 * - Intrusive.
 *
 *
 * Usage
 * -----
 *
 * @code
 * typedef SomeData MyData;
 *
 * CN_DICT(struct MyDict, MyData);
 *
 * void push(struct MyDict** dictp, const char* key, MyData val)
 * {
 *     struct MyDict* entry = malloc(sizeof(*entry));
 *
 *     cn_dict_setk(entry, key);
 *     *cn_dict_data(entry) = val;
 *     *dictp = cn_dict_ins(*dictp, entry);
 * }
 * @endcode
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
 *
 * @brief Get the pointer to the CnStrnode member of the dictionary entry.
 *
 * @param[in] dict The dictionary entry.
 *
 * @return A pointer to the CnStrnode.
 *
 * @note Compile with the GNU extension to enable a type check for the @a dict.
 */
#define cn_dict_cast(dict)                                                     \
	({                                                                     \
		__typeof__(dict) _dict2 = (dict);                              \
                                                                               \
		_dict2 ? &_dict2->strnode : NULL;                              \
	})

/**
 * @def cn_dict_setk(dict, key)
 *
 * @brief Set a string key for a dictionary entry.
 *
 * @param[in] dict The entry.
 * @param[in] key The string key.
 *
 * @note Compile with the GNU extension to enable a type check for the @a dict.
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
 *
 * @brief Get the string key from a dictionary entry.
 *
 * @param[in] dict The entry.
 *
 * @return A string key.
 *
 * @note Compile with the GNU extension to enable a type check for the @a dict.
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
 *
 * @brief Get the pointer to the data member of the dictionary entry.
 *
 * @param[in] dict The entry.
 *
 * @return A pointer to the data.
 *
 * @note Compile with the GNU extension to enable a type check for the @a dict.
 */
#define cn_dict_data(dict)                                                     \
	({                                                                     \
		__typeof__(dict) _dict = (dict);                               \
                                                                               \
		CN_ENSURE(_dict, CN_ERROR, null_param);                        \
		&_dict->data;                                                  \
	})

/**
 * @def cn_dict_ins(dict, entry)
 *
 * @brief Insert an entry into the dictionary.
 *
 * @param[in,out] dict The root.
 * @param[in,out] entry The new entry.
 *
 * @return The new root.
 *
 * @note Compile with the GNU extension to enable a type check for the @a dict.
 */
#define cn_dict_ins(dict, entry)                                               \
	({                                                                     \
		__typeof__(dict) _dict = (dict);                               \
                                                                               \
		cn_container_of(                                               \
			cn_strnode_ins(                                        \
				cn_dict_cast(_dict), cn_dict_cast(entry)),     \
			__typeof__(*dict), strnode);                           \
	})

/**
 * @def cn_dict_find(dict, key)
 *
 * @brief In a dictionary, find the entry with the given key.
 *
 * @param[in] dict The root.
 * @param[in] key The string key.
 *
 * @return The found entry or NULL if none found.
 *
 * @note Compile with the GNU extension to enable a type check for the @a dict.
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
 *
 * @brief Find the first entry of the dictionary traversal.
 *
 * @param[in] dict Any entry from the dictionary.
 *
 * For the lexicographical order of the dictionary keys, find the entry with
 * the first key.
 *
 * @return The first entry.
 *
 * @note Compile with the GNU extension to enable a type check for the @a dict.
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
 *
 * @brief Find the next entry of the dictionary traversal.
 *
 * For the lexicographical order of the dictionary keys, find the entry with
 * the next key.
 *
 * @param[in] dict The entry.
 *
 * @return The next entry.
 *
 * @note Compile with the GNU extension to enable a type check for the @a dict.
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
 *
 * @brief Define the dictionary.
 *
 * @param[in] name The name of the type used for dictionary.
 * @param[in] type The type of the data held by @a name.
 *
 * This macro will define a compound type (must be struct or union) @a name,
 * a type for a dictionary entry that holds the data of the type @a type.
 */
#define CN_DICT(name, type)                                                    \
	name                                                                   \
	{                                                                      \
		struct CnStrnode strnode;                                      \
		type data;                                                     \
	}

/**
 * @struct CnStrnode
 *
 * @brief Dictionary node.
 */
struct CnStrnode {
	/**
	 * @var struct CnRbnode node
	 *
	 * @brief The red-black tree node.
	 */
	struct CnRbnode node;

	/**
	 * @var char* str
	 *
	 * @brief The string key.
	 */
	char* str;
};

/**
 * @fn struct CnStrnode* cn_strnode_ins(struct CnStrnode* root, struct CnStrnode* entry)
 *
 * @brief Insert an entry into the dictionary.
 *
 * @param[in,out] root The root.
 * @param[in,out] entry The new entry.
 *
 * @return The new entry.
 */
struct CnStrnode*
cn_strnode_ins(struct CnStrnode* root, struct CnStrnode* entry);

/**
 * @fn struct CnStrnode* cn_strnode_find(struct CnStrnode* root, const char* str)
 *
 * @brief In a dictionary, find the entry with the given key.
 *
 * @param[in] root The root.
 * @param[in] str The string key.
 *
 * @return The found entry or NULL if none found.
 */
struct CnStrnode* cn_strnode_find(struct CnStrnode* root, const char* str);

/**
 * @fn static inline struct CnStrnode* cn_strnode_from(struct CnRbnode* ptr)
 *
 * @brief Cast a CnRbnode member out to the containing CnStrnode structure.
 *
 * @param[in] ptr The pointer to the CnRbnode member.
 *
 * @return A pointer to the CnStrnode structure.
 */
static inline struct CnStrnode* cn_strnode_from(struct CnRbnode* ptr)
{
	return cn_container_of(ptr, struct CnStrnode, node);
}

#endif /* CN_DICT_H */
