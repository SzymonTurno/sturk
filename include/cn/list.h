#ifndef CN_LIST_H
#define CN_LIST_H

#include "cn/logger/except.h"

struct CnUnnode {
	struct CnUnnode* next;
};

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

#else /* __STRICT_ANSI__ */

#define CN_LIST(name, type)                                                    \
	name                                                                   \
	{                                                                      \
		union {                                                        \
			name* next;                                            \
			struct CnUnnode node;                                  \
		};                                                             \
		type data;                                                     \
	}

#endif /* __STRICT_ANSI__ */

#define CN_LIST_ITER(type, i, ...) _CN_LIST_ITER(type, (i), __VA_ARGS__, 1, )

struct CnUnnode** cn_unnode_hand(struct CnUnnode** nodep, int pos);

struct CnUnnode*
cn_unnode_ins(struct CnUnnode* head, struct CnUnnode* node, int pos);

struct CnUnnode* cn_unnode_rem(struct CnUnnode** headp, int pos);

#ifdef __STRICT_ANSI__

#define cn_list_data(list) (&(list)->data)

#else /* __STRICT_ANSI__ */

#define cn_list_data(list)                                                     \
	({                                                                     \
		__typeof__(list) _list = (list);                               \
                                                                               \
		CN_ENSURE(_list, CN_ERROR, null_param);                        \
		&(_list)->data;                                                \
	})

#endif /* __STRICT_ANSI__ */

#define cn_list_ins(list, ...) _CN_LIST_INS((list), __VA_ARGS__, 0, )

#define cn_list_rem(...) _CN_LIST_REM(__VA_ARGS__, 0, )

#ifdef __STRICT_ANSI__

#define cn_list_hand(listp, pos)                                               \
	((void*)cn_unnode_hand((struct CnUnnode**)listp, pos))

#else /* __STRICT_ANSI__ */

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

#endif /* __STRICT_ANSI__ */

#define _CN_LIST_ITER(type, _i, listp, pos, ...)                               \
	type** _i = (listp);                                                   \
	*_i;                                                                   \
	_i = cn_list_hand(_i, (pos))

#ifdef __STRICT_ANSI__

#define _CN_LIST_INS(list, entry, pos, ...)                                    \
	((void*)cn_unnode_ins(                                                 \
		(struct CnUnnode*)(list), (struct CnUnnode*)(entry), (pos)))

#else /* __STRICT_ANSI__ */

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

#endif /* __STRICT_ANSI__ */

#ifdef __STRICT_ANSI__

#define _CN_LIST_REM(listp, pos, ...)                                          \
	((void*)cn_unnode_rem((struct CnUnnode**)(listp), (pos)))

#else /* __STRICT_ANSI__ */

#define _CN_LIST_REM(listp, pos, ...)                                          \
	({                                                                     \
		__typeof__(listp) _tmp = cn_list_hand((listp), (pos));         \
		__typeof__(*_tmp) _ret = *_tmp;                                \
                                                                               \
		*_tmp = (*_tmp)->next;                                         \
		_ret;                                                          \
	})

#endif /* __STRICT_ANSI__ */

#endif /* CN_LIST_H */
