/*
BSD 3-Clause License

Copyright (c) 2025, Szymon Turno

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

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

#define cn_dict_cast(dict) ((struct CnDictnode*)(dict))

#define cn_dict_setk(dict, key) ((dict)->dictnode.str = (key))

#define cn_dict_getk(dict) ((dict)->dictnode.str)

#define cn_dict_datap(dict) (&(dict)->data)

#define cn_dict_ins(dict, node)                                                \
	((void*)cn_dictnode_ins(cn_dict_cast(dict), cn_dict_cast(node)))

#define cn_dict_find(dict, key)                                                \
	((void*)cn_dictnode_find(cn_dict_cast(dict), (key)))

#define cn_dict_first(dict)                                                    \
	((void*)(cn_dictnode_from(cn_rb_first(&dict_cast(dict)->node, 0))))

#define cn_dict_next(dict)                                                     \
	((void*)(cn_dictnode_from(cn_rb_next(&dict_cast(dict)->node, 0))))

#else /* not defined: __STRICT_ANSI__ */

/**
 * @def cn_dict_cast(dict)
 *
 * @brief Get the pointer to the CnDictnode member of the dictionary entry.
 *
 * @param[in] dict The dictionary entry.
 *
 * @return A pointer to the CnDictnode.
 *
 * @note Compile with the GNU extension to enable a type check for the @a dict.
 */
#define cn_dict_cast(dict)                                                     \
	({                                                                     \
		__typeof__(dict) _dict2 = (dict);                              \
                                                                               \
		_dict2 ? &_dict2->dictnode : NULL;                             \
	})

/**
 * @def cn_dict_setk(dict, key)
 *
 * @brief Set a key string for a dictionary entry.
 *
 * @param[in] dict The entry.
 * @param[in] key The key string.
 *
 * @note Compile with the GNU extension to enable a type check for the @a dict.
 */
#define cn_dict_setk(dict, key)                                                \
	({                                                                     \
		__typeof__(dict) _dict = (dict);                               \
                                                                               \
		CN_ENSURE(_dict, CN_ERROR, null_param);                        \
		_dict->dictnode.str = (key);                                   \
	})

/**
 * @def cn_dict_getk(dict)
 *
 * @brief Get the key string from a dictionary entry.
 *
 * @param[in] dict The entry.
 *
 * @return A key string.
 *
 * @note Compile with the GNU extension to enable a type check for the @a dict.
 */
#define cn_dict_getk(dict)                                                     \
	({                                                                     \
		__typeof__(dict) _dict = (dict);                               \
                                                                               \
		CN_ENSURE(_dict, CN_ERROR, null_param);                        \
		_dict->dictnode.str;                                           \
	})

/**
 * @def cn_dict_datap(dict)
 *
 * @brief Get a pointer to the data member of a dictionary entry.
 *
 * @param[in] dict The entry.
 *
 * @return The pointer to the data.
 *
 * @note Compile with the GNU extension to enable a type check for the @a dict.
 */
#define cn_dict_datap(dict)                                                    \
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
			cn_dictnode_ins(                                       \
				cn_dict_cast(_dict), cn_dict_cast(entry)),     \
			__typeof__(*dict), dictnode);                          \
	})

/**
 * @def cn_dict_find(dict, key)
 *
 * @brief In a dictionary, find the entry with the given key.
 *
 * @param[in] dict The root.
 * @param[in] key The key string.
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
			cn_dictnode_find(cn_dict_cast(_dict), (key)),          \
			__typeof__(*dict), dictnode);                          \
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
			cn_dictnode_from(                                      \
				cn_rb_first(&dict_cast(_dict)->node, 0)),      \
			__typeof__(*dict), dictnode);                          \
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
			cn_dictnode_from(cn_rb_next(                           \
				&dict_cast(_dict)->node, CN_BST_INORDER)),     \
			__typeof__(*dict), dictnode);                          \
	})

#endif /* __STRICT_ANSI__ */

/**
 * @def CN_DICT(name, type)
 *
 * @brief Define the dictionary.
 *
 * @param[in] name The name of the type used for the dictionary.
 * @param[in] type The type of the data held by @a name.
 *
 * This macro will define a compound type (must be struct or union) @a name,
 * a type for a dictionary entry that holds the data of the type @a type.
 */
#define CN_DICT(name, type)                                                    \
	name                                                                   \
	{                                                                      \
		struct CnDictnode dictnode;                                    \
		type data;                                                     \
	}

/**
 * @struct CnDictnode
 *
 * @brief Dictionary node.
 */
struct CnDictnode {
	/**
	 * @var struct CnRbnode node
	 *
	 * @brief The red-black tree node.
	 */
	struct CnRbnode node;

	/**
	 * @var char* str
	 *
	 * @brief The key string.
	 */
	char* str;
};

/**
 * @fn struct CnDictnode* cn_dictnode_ins(struct CnDictnode* root, struct CnDictnode* entry)
 *
 * @brief Insert an entry into the dictionary.
 *
 * @param[in,out] root The root.
 * @param[in,out] entry The new entry.
 *
 * @return The new entry.
 */
struct CnDictnode*
cn_dictnode_ins(struct CnDictnode* root, struct CnDictnode* entry);

/**
 * @fn struct CnDictnode* cn_dictnode_find(struct CnDictnode* root, const char* str)
 *
 * @brief In a dictionary, find the entry with the given key.
 *
 * @param[in] root The root.
 * @param[in] str The key string.
 *
 * @return The found entry or NULL if none found.
 */
struct CnDictnode* cn_dictnode_find(struct CnDictnode* root, const char* str);

/**
 * @fn static inline struct CnDictnode* cn_dictnode_from(struct CnRbnode* ptr)
 *
 * @brief Cast a CnRbnode member out to the containing CnDictnode structure.
 *
 * @param[in] ptr The pointer to the CnRbnode member.
 *
 * @return A pointer to the CnDictnode structure.
 */
static inline struct CnDictnode* cn_dictnode_from(struct CnRbnode* ptr)
{
	return cn_container_of(ptr, struct CnDictnode, node);
}

#endif /* CN_DICT_H */
