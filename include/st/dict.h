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
 * @file st/dict.h
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
 * ST_DICT(struct MyDict, MyData);
 *
 * void push(struct MyDict** dictp, const char* key, MyData val)
 * {
 *     struct MyDict* entry = malloc(sizeof(*entry));
 *
 *     st_dict_setk(entry, key);
 *     *st_dict_data(entry) = val;
 *     *dictp = st_dict_ins(*dictp, entry);
 * }
 * @endcode
 */

#ifndef ST_DICT_H
#define ST_DICT_H

#include "st/arith.h"
#include "st/logger/except.h"
#include "st/rbtree.h"

#ifdef __STRICT_ANSI__

#define st_dict_cast(dict) ((struct StDictnode*)(dict))

#define st_dict_setk(dict, key) ((dict)->dictnode.str = (key))

#define st_dict_getk(dict) ((dict)->dictnode.str)

#define st_dict_datap(dict) (&(dict)->data)

#define st_dict_ins(dict, node)                                                \
	((void*)st_dictnode_ins(st_dict_cast(dict), st_dict_cast(node)))

#define st_dict_find(dict, key)                                                \
	((void*)st_dictnode_find(st_dict_cast(dict), (key)))

#define st_dict_first(dict)                                                    \
	((void*)(st_dictnode_from(st_rb_first(&dict_cast(dict)->node, 0))))

#define st_dict_next(dict)                                                     \
	((void*)(st_dictnode_from(st_rb_next(&dict_cast(dict)->node, 0))))

#else /* not defined: __STRICT_ANSI__ */

/**
 * @def st_dict_cast(dict)
 *
 * @brief Get the pointer to the StDictnode member of the dictionary entry.
 *
 * @param[in] dict The dictionary entry.
 *
 * @return A pointer to the StDictnode.
 *
 * @note Compile with the GNU extension to enable a type check for the @a dict.
 */
#define st_dict_cast(dict)                                                     \
	({                                                                     \
		__typeof__(dict) _dict2 = (dict);                              \
                                                                               \
		_dict2 ? &_dict2->dictnode : NULL;                             \
	})

/**
 * @def st_dict_setk(dict, key)
 *
 * @brief Set a key string for a dictionary entry.
 *
 * @param[in] dict The entry.
 * @param[in] key The key string.
 *
 * @note Compile with the GNU extension to enable a type check for the @a dict.
 */
#define st_dict_setk(dict, key)                                                \
	({                                                                     \
		__typeof__(dict) _dict = (dict);                               \
                                                                               \
		ST_ENSURE(_dict, ST_ERROR, null_param);                        \
		_dict->dictnode.str = (key);                                   \
	})

/**
 * @def st_dict_getk(dict)
 *
 * @brief Get the key string from a dictionary entry.
 *
 * @param[in] dict The entry.
 *
 * @return A key string.
 *
 * @note Compile with the GNU extension to enable a type check for the @a dict.
 */
#define st_dict_getk(dict)                                                     \
	({                                                                     \
		__typeof__(dict) _dict = (dict);                               \
                                                                               \
		ST_ENSURE(_dict, ST_ERROR, null_param);                        \
		_dict->dictnode.str;                                           \
	})

/**
 * @def st_dict_datap(dict)
 *
 * @brief Get a pointer to the data member of a dictionary entry.
 *
 * @param[in] dict The entry.
 *
 * @return The pointer to the data.
 *
 * @note Compile with the GNU extension to enable a type check for the @a dict.
 */
#define st_dict_datap(dict)                                                    \
	({                                                                     \
		__typeof__(dict) _dict = (dict);                               \
                                                                               \
		ST_ENSURE(_dict, ST_ERROR, null_param);                        \
		&_dict->data;                                                  \
	})

/**
 * @def st_dict_ins(dict, entry)
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
#define st_dict_ins(dict, entry)                                               \
	({                                                                     \
		__typeof__(dict) _dict = (dict);                               \
                                                                               \
		st_container_of(                                               \
			st_dictnode_ins(                                       \
				st_dict_cast(_dict), st_dict_cast(entry)),     \
			__typeof__(*dict), dictnode);                          \
	})

/**
 * @def st_dict_find(dict, key)
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
#define st_dict_find(dict, key)                                                \
	({                                                                     \
		__typeof__(dict) _dict = (dict);                               \
                                                                               \
		st_container_of(                                               \
			st_dictnode_find(st_dict_cast(_dict), (key)),          \
			__typeof__(*dict), dictnode);                          \
	})

/**
 * @def st_dict_first(dict)
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
#define st_dict_first(dict)                                                    \
	({                                                                     \
		__typeof__(dict) _dict = (dict);                               \
                                                                               \
		st_container_of(                                               \
			st_dictnode_from(                                      \
				st_rb_first(&dict_cast(_dict)->node, 0)),      \
			__typeof__(*dict), dictnode);                          \
	})

/**
 * @def st_dict_next(dict)
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
#define st_dict_next(dict)                                                     \
	({                                                                     \
		__typeof__(dict) _dict = (dict);                               \
                                                                               \
		st_container_of(                                               \
			st_dictnode_from(st_rb_next(                           \
				&dict_cast(_dict)->node, ST_BST_INORDER)),     \
			__typeof__(*dict), dictnode);                          \
	})

#endif /* __STRICT_ANSI__ */

/**
 * @def ST_DICT(name, type)
 *
 * @brief Define the dictionary.
 *
 * @param[in] name The name of the type used for the dictionary.
 * @param[in] type The type of the data held by @a name.
 *
 * This macro will define a compound type (must be struct or union) @a name,
 * a type for a dictionary entry that holds the data of the type @a type.
 */
#define ST_DICT(name, type)                                                    \
	name                                                                   \
	{                                                                      \
		struct StDictnode dictnode;                                    \
		type data;                                                     \
	}

/**
 * @struct StDictnode
 *
 * @brief Dictionary node.
 */
struct StDictnode {
	/**
	 * @var struct StRbnode node
	 *
	 * @brief The red-black tree node.
	 */
	struct StRbnode node;

	/**
	 * @var char* str
	 *
	 * @brief The key string.
	 */
	char* str;
};

/**
 * @fn struct StDictnode* st_dictnode_ins(struct StDictnode* root, struct StDictnode* entry)
 *
 * @brief Insert an entry into the dictionary.
 *
 * @param[in,out] root The root.
 * @param[in,out] entry The new entry.
 *
 * @return The new entry.
 */
struct StDictnode*
st_dictnode_ins(struct StDictnode* root, struct StDictnode* entry);

/**
 * @fn struct StDictnode* st_dictnode_find(struct StDictnode* root, const char* str)
 *
 * @brief In a dictionary, find the entry with the given key.
 *
 * @param[in] root The root.
 * @param[in] str The key string.
 *
 * @return The found entry or NULL if none found.
 */
struct StDictnode* st_dictnode_find(struct StDictnode* root, const char* str);

/**
 * @fn static inline struct StDictnode* st_dictnode_from(struct StRbnode* ptr)
 *
 * @brief Cast a StRbnode member out to the containing StDictnode structure.
 *
 * @param[in] ptr The pointer to the StRbnode member.
 *
 * @return A pointer to the StDictnode structure.
 */
static inline struct StDictnode* st_dictnode_from(struct StRbnode* ptr)
{
	return st_container_of(ptr, struct StDictnode, node);
}

#endif /* ST_DICT_H */
