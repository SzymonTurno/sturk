#ifndef UB_LIST_H
#define UB_LIST_H

#define UB_LIST(name, type) name {name* next; type data;}

#define UB_LIST_ITER(i, ...) _UB_LIST_ITER((i), __VA_ARGS__, 1)

#define ub_list_data(list) (&(list)->data)

#define ub_list_ins(list, ...) _UB_LIST_INS((list), __VA_ARGS__, 0,)

#define ub_list_rem(...) _UB_LIST_REM(__VA_ARGS__, 0,)

#define ub_list_hand(listp, pos)                                              \
	({                                                                     \
		__typeof__(listp) _listp = (listp);                            \
		long long _pos = (pos);                                        \
									       \
		for (; *_listp && _pos--; _listp = &(*_listp)->next)           \
			;                                                      \
		_listp;                                                        \
	})

#define _UB_LIST_ITER(_i, listp, pos, ...)                                    \
	__typeof__(listp) _i = (listp); *_i; _i = ub_list_hand(_i, (pos))

#define _UB_LIST_INS(list, entry, pos, ...)                                   \
	({                                                                     \
		__typeof__(list) _list = (list);                               \
		__typeof__(entry) _entry = (entry);                            \
		__typeof__(&list) _tmp = ub_list_hand(&_list, (pos));          \
									       \
		_entry->next = *_tmp;                                          \
		*_tmp = _entry;                                                \
		_list;                                                         \
	})

#define _UB_LIST_REM(listp, pos, ...)                                         \
	({                                                                     \
		__typeof__(listp) _tmp = ub_list_hand((listp), (pos));         \
		__typeof__(*_tmp) _ret = *_tmp;                                \
									       \
		*_tmp = (*_tmp)->next;                                         \
		_ret;                                                          \
	})

#endif /* UB_LIST_H */
