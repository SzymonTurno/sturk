#ifndef UB_DICT_H
#define UB_DICT_H

#include "ub/rbtree.h"
#include "ub/arith.h"
#include "ub/debug.h"
#include <string.h>

#define UB_DICT(name, type)                                                   \
	name {struct UBrbnode node; char* key; type val;}

#define ub_dict_cast(dict)                                                    \
	({                                                                     \
		__typeof__(dict) _mdict = (dict);                              \
									       \
		ub_ensure(_mdict, "Bad pointer.");                             \
		(&(_mdict)->node);                                             \
	})

#define ub_dict_setk(dict, str)                                               \
	({                                                                     \
		__typeof__(dict) _mdict = (dict);                              \
									       \
		ub_ensure(_mdict, "Bad pointer.");                             \
		((_mdict)->key = (str));                                       \
	})

#define ub_dict_getk(dict)                                                    \
	({                                                                     \
		__typeof__(dict) _mdict = (dict);                              \
									       \
		ub_ensure(_mdict, "Bad pointer.");                             \
		((_mdict)->key);                                               \
	})

#define ub_dict_data(dict)                                                    \
	({                                                                     \
		__typeof__(dict) _mdict = (dict);                              \
									       \
		ub_ensure(_mdict, "Bad pointer.");                             \
		(&(_mdict)->val);                                              \
	})

#define ub_dict_cont(ptr, type) ub_container_of(ptr, type, node)

#define ub_dict_ins(dict, entry)                                              \
	({                                                                     \
		__typeof__(dict) _dict = (dict);                               \
		__typeof__(entry) _entry = (entry);                            \
		const char* _key = _entry->key;                                \
		struct UBrbnode* _root = _dict ? &dict->node : NULL;           \
		struct UBrbnode** _i = &_root;                                 \
		struct UBrbnode* _p = NULL;                                    \
		struct UBrbnode* _node = _entry ? &_entry->node : NULL;        \
									       \
		while (*_i) {                                                  \
			_p = *_i;                                              \
			if (strcmp(_key, ub_dict_cont(_p,                      \
				__typeof__(*dict))->key) < 0)                  \
				_i = &_p->left;                                \
			else                                                   \
				_i = &_p->right;                               \
		}                                                              \
		*_i = ub_rb_link(_node, _p);                                   \
		_root = ub_rb_insrebal(_root, _node);                          \
		(__typeof__(dict))_root;                                       \
	})

#define ub_dict_find(dict, str)                                               \
	({                                                                     \
		const char* _key = (str);                                      \
		__typeof__(dict) _dict = (dict);                               \
		struct UBrbnode* _node = NULL;                                 \
		int _tmp = 0;                                                  \
									       \
		ub_ensure(_key, "Bad pointer.");                               \
		while (_dict) {                                                \
			_tmp = strcmp(_key, _dict->key);                       \
			if (_tmp < 0) {                                        \
				_node = _dict->node.left;                      \
				_dict = ub_dict_cont(_node, __typeof__(*dict));\
			} else if (_tmp > 0) {                                 \
				_node = _dict->node.right;                     \
				_dict = ub_dict_cont(_node, __typeof__(*dict));\
			} else {                                               \
				break;                                         \
			}                                                      \
		}                                                              \
		_dict;                                                         \
	})

#endif /* UB_DICT_H */
