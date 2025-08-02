/**
 * @file cn/list.h
 *
 * @brief Singly linked list.
 *
 * This header file provides data types, functions and macros that define and
 * operate on singly linked lists.
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
 * CN_LIST(struct MyList, MyData);
 *
 * void push(struct MyList** listp, MyData data)
 * {
 *     struct MyList* entry = malloc(sizeof(*entry));
 *
 *     *cn_list_data(entry) = data;
 *     *listp = cn_list_ins(*listp, entry);
 * }
 * @endcode
 */

#ifndef CN_LIST_H
#define CN_LIST_H

#include "cn/logger/except.h"

#ifdef __STRICT_ANSI__

#define CN_LIST(name, type)                                                    \
	name                                                                   \
	{                                                                      \
		union {                                                        \
			name* next;                                            \
			struct CnUnnode node;                                  \
		} u;                                                           \
		type data;                                                     \
	}

#define cn_list_data(list) (&(list)->data)

#define cn_list_hand(listp, pos)                                               \
	((void*)cn_unnode_hand((struct CnUnnode**)listp, pos))

#define _cn_list_iter(_i, listp, pos, ...)                                     \
	for (struct _CnVoidList** _i = (struct _CnVoidList**)(listp); *_i;     \
	     _i = cn_list_hand(_i, (pos)))

#define _CN_LIST_INS(list, entry, pos, ...)                                    \
	((void*)cn_unnode_ins(                                                 \
		(struct CnUnnode*)(list), (struct CnUnnode*)(entry), (pos)))

#define _CN_LIST_REM(listp, pos, ...)                                          \
	((void*)cn_unnode_rem((struct CnUnnode**)(listp), (pos)))

#else /* not defined: __STRICT_ANSI__ */

/**
 * @def CN_LIST(name, type)
 *
 * @brief Define the list.
 *
 * @param[in] name The name of the type used for the list.
 * @param[in] type The type of the data held by @a name.
 *
 * This macro will define a compound type (must be struct or union) @a name,
 * a type for a list entry that holds the data of the type @a type.
 */
#define CN_LIST(name, type)                                                    \
	name                                                                   \
	{                                                                      \
		union {                                                        \
			name* next;                                            \
			struct CnUnnode node;                                  \
		};                                                             \
		type data;                                                     \
	}

/**
 * @def cn_list_data(list)
 *
 * @brief Get the pointer to the data member of the list entry.
 *
 * @param[in] list The entry.
 *
 * @return A pointer to the data.
 *
 * @note Compile with the GNU extension to enable a type check for the @a list.
 */
#define cn_list_data(list)                                                     \
	({                                                                     \
		__typeof__(list) _list = (list);                               \
                                                                               \
		CN_ENSURE(_list, CN_ERROR, null_param);                        \
		&(_list)->data;                                                \
	})

/**
 * @def cn_list_hand(listp, pos)
 *
 * @brief Get a double pointer to the list entry at the given posistion.
 *
 * @param[in] listp The double pointer to the head.
 * @param[in] pos The position.
 *
 * @return The double pointer to the list entry.
 *
 * @note The returned double pointer can be used to directly modify the list.
 * @note Compile with the GNU extension to enable a type check for the @a listp.
 */
#define cn_list_hand(listp, pos)                                               \
	({                                                                     \
		__typeof__(listp) _listp = (listp);                            \
		long long _pos = (pos);                                        \
                                                                               \
		CN_ENSURE(_listp, CN_ERROR, null_param);                       \
		for (; *_listp && _pos--; _listp = &(*_listp)->next)           \
			;                                                      \
		_listp;                                                        \
	})

/* @cond */
#define _cn_list_iter(_i, listp, pos, ...)                                     \
	for (__typeof__(listp) _i = (listp); *_i; _i = cn_list_hand(_i, (pos)))

#define _CN_LIST_INS(list, entry, pos, ...)                                    \
	({                                                                     \
		__typeof__(entry) _list = (list);                              \
		__typeof__(entry) _entry = (entry);                            \
		__typeof__(&list) _i = cn_list_hand(&_list, (pos));            \
                                                                               \
		CN_ENSURE(_entry, CN_ERROR, null_param);                       \
		_entry->next = *_i;                                            \
		*_i = _entry;                                                  \
		_list;                                                         \
	})

#define _CN_LIST_REM(listp, pos, ...)                                          \
	({                                                                     \
		__typeof__(listp) _tmp = cn_list_hand((listp), (pos));         \
		__typeof__(*_tmp) _ret = *_tmp;                                \
                                                                               \
		*_tmp = (*_tmp)->next;                                         \
		_ret;                                                          \
	})
/* @endcond */

#endif /* __STRICT_ANSI__ */

/**
 * @def cn_list_iter(i, ...)
 *
 * @brief Traverse the list.
 *
 * A call cn_list_iter(i, litsp, pos) will traverse the list referenced by @a
 * listp in the forward direcion, assigning every @a pos entry in turn to @a
 * i. The @a pos argument is optional and by default it equals 1.
 *
 * @note Compile with the GNU extension to enable a type check for the @a listp.
 */
#define cn_list_iter(i, ...) _cn_list_iter ((i), __VA_ARGS__, 1, )

/**
 * @def cn_list_ins(list, ...)
 *
 * @brief Insert, at the given position, an entry into the list.
 *
 * A call cn_list_ins(list, entry, pos) will insert the @a entry into the @a
 * list at the @a pos. To insert at the head use 0 for the @a pos and to insert
 * at the tail use -1 for the @a pos. The argument @a pos is optional and by
 * default it equals 0.
 *
 * @note Compile with the GNU extension to enable a type check for the @a list.
 */
#define cn_list_ins(list, ...) _CN_LIST_INS((list), __VA_ARGS__, 0, )

/**
 * @def cn_list_rem(...)
 *
 * @brief Remove, at the given position, an entry from the list.
 *
 * A call cn_list_rem(listp, pos) will remove an entry from the @a listp at the
 * @a pos and will return the removed entry. To remove from the head use 0 for
 * the @a pos and to remove from the tail use -1 for the @a pos. The argument
 * @a pos is optional and by default it equals 0.
 *
 * @return The removed entry.
 *
 * @note Compile with the GNU extension to enable a type check for the @a listp.
 */
#define cn_list_rem(...) _CN_LIST_REM(__VA_ARGS__, 0, )

/**
 * @struct CnUnnode
 *
 * @brief Singly linked list node.
 */
struct CnUnnode {
	/**
	 * @var struct CnUnnode* next
	 *
	 * @brief The pointer to the next entry in the singly linked list.
	 */
	struct CnUnnode* next;
};

/* @cond */
CN_LIST(struct _CnVoidList, void*);
/* @endcond */

/**
 * @fn struct CnUnnode** cn_unnode_hand(struct CnUnnode** nodep, int pos)
 *
 * @brief Get a double pointer to the list entry at the given posistion.
 *
 * @param[in] nodep The double pointer to the head.
 * @param[in] pos The position.
 *
 * @return The double pointer to the list entry.
 *
 * @note The returned double pointer can be used to directly modify the list.
 */
struct CnUnnode** cn_unnode_hand(struct CnUnnode** nodep, int pos);

/**
 * @fn struct CnUnnode* cn_unnode_ins(struct CnUnnode* head, struct CnUnnode* node, int pos)
 *
 * @brief Insert, at the given position, an entry into the list.
 *
 * @param[in,out] head The head of the list.
 * @param[in,out] node The new entry.
 * @param[in] pos The position at which the new entry is inserted.
 *
 * @return The new head.
 */
struct CnUnnode*
cn_unnode_ins(struct CnUnnode* head, struct CnUnnode* node, int pos);

/**
 * @fn struct CnUnnode* cn_unnode_rem(struct CnUnnode** headp, int pos)
 *
 * @brief Remove, at the given position, an entry from the list.
 *
 * @param[in,out] headp The double pointer to the head of the list.
 * @param[in] pos The position of the entry that is removed.
 *
 * @return The removed entry.
 */
struct CnUnnode* cn_unnode_rem(struct CnUnnode** headp, int pos);

#endif /* CN_LIST_H */
