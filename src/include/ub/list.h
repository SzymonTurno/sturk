#ifndef UB_LIST_H
#define UB_LIST_H

#include "ub/logger/except.h"

struct UBunnode {
	struct UBunnode* next;
};

#ifdef __STRICT_ANSI__
#define UB_LIST(name, type)                                                   \
	name { union { name* next; struct UBunnode node; } u; type data; }
#else /* __STRICT_ANSI__ */
#define UB_LIST(name, type)                                                   \
	name { union { name* next; struct UBunnode node; }; type data; }
#endif /* __STRICT_ANSI__ */

#define UB_LIST_ITER(type, i, ...) _UB_LIST_ITER(type, (i), __VA_ARGS__, 1,)

struct UBunnode** ub_unnode_hand(struct UBunnode** nodep, int pos);

struct UBunnode* ub_unnode_ins(struct UBunnode* head, struct UBunnode* node,
	int pos);

struct UBunnode* ub_unnode_rem(struct UBunnode** headp, int pos);

#ifdef __STRICT_ANSI__
#define ub_list_data(list) (&(list)->data)
#else /* __STRICT_ANSI__ */
#define ub_list_data(list)                                                    \
	({                                                                     \
		__typeof__(list) _list = (list);                               \
									       \
		UB_ENSURE(_list, UB_ECODES.null_param);                        \
		&(_list)->data;                                                \
	})
#endif /* __STRICT_ANSI__ */

#define ub_list_ins(list, ...) _UB_LIST_INS((list), __VA_ARGS__, 0,)

#define ub_list_rem(...) _UB_LIST_REM(__VA_ARGS__, 0,)

#ifdef __STRICT_ANSI__
#define ub_list_hand(listp, pos)                                              \
	((void*)ub_unnode_hand((struct UBunnode**)listp, pos))
#else /* __STRICT_ANSI__ */
#define ub_list_hand(listp, pos)                                              \
	({                                                                     \
		__typeof__(listp) _listp = (listp);                            \
		long long _pos = (pos);                                        \
									       \
		UB_ENSURE(_listp, UB_ECODES.null_param);                       \
		for (; *_listp && _pos--; _listp = &(*_listp)->next)           \
			;                                                      \
		_listp;                                                        \
	})
#endif /* __STRICT_ANSI__ */

#define _UB_LIST_ITER(type, _i, listp, pos, ...)                              \
	type** _i = (listp); *_i; _i = ub_list_hand(_i, (pos))

#ifdef __STRICT_ANSI__
#define _UB_LIST_INS(list, entry, pos, ...)                                   \
	((void*)ub_unnode_ins((struct UBunnode*)(list),                        \
		(struct UBunnode*)(entry), (pos)))
#else /* __STRICT_ANSI__ */
#define _UB_LIST_INS(list, entry, pos, ...)                                   \
	({                                                                     \
		__typeof__(entry) _list = (list);                              \
		__typeof__(entry) _entry = (entry);                            \
		__typeof__(&list) _i = ub_list_hand(&_list, (pos));            \
									       \
		UB_ENSURE(_entry, UB_ECODES.null_param);                       \
		_entry->next = *_i;                                            \
		*_i = _entry;                                                  \
		_list;                                                         \
	})
#endif /* __STRICT_ANSI__ */

#ifdef __STRICT_ANSI__
#define _UB_LIST_REM(listp, pos, ...)                                         \
	((void*)ub_unnode_rem((struct UBunnode**)(listp), (pos)))
#else /* __STRICT_ANSI__ */
#define _UB_LIST_REM(listp, pos, ...)                                         \
	({                                                                     \
		__typeof__(listp) _tmp = ub_list_hand((listp), (pos));         \
		__typeof__(*_tmp) _ret = *_tmp;                                \
									       \
		*_tmp = (*_tmp)->next;                                         \
		_ret;                                                          \
	})
#endif /* __STRICT_ANSI__ */

#endif /* UB_LIST_H */
