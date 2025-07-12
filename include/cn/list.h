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

#define CN_LIST(name, type)                                                    \
	name                                                                   \
	{                                                                      \
		union {                                                        \
			name* next;                                            \
			struct CnUnnode node;                                  \
		};                                                             \
		type data;                                                     \
	}

#define cn_list_data(list)                                                     \
	({                                                                     \
		__typeof__(list) _list = (list);                               \
                                                                               \
		CN_ENSURE(_list, CN_ERROR, null_param);                        \
		&(_list)->data;                                                \
	})

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

#endif /* __STRICT_ANSI__ */

#define cn_list_iter(i, ...) _cn_list_iter ((i), __VA_ARGS__, 1, )

#define cn_list_ins(list, ...) _CN_LIST_INS((list), __VA_ARGS__, 0, )

#define cn_list_rem(...) _CN_LIST_REM(__VA_ARGS__, 0, )

struct CnUnnode {
	struct CnUnnode* next;
};

CN_LIST(struct _CnVoidList, void*);

struct CnUnnode** cn_unnode_hand(struct CnUnnode** nodep, int pos);

struct CnUnnode*
cn_unnode_ins(struct CnUnnode* head, struct CnUnnode* node, int pos);

struct CnUnnode* cn_unnode_rem(struct CnUnnode** headp, int pos);

#endif /* CN_LIST_H */
