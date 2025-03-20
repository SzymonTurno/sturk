#ifndef UB_DICT_H
#define UB_DICT_H

#include "ub/rbtree.h"
#include <string.h>

#define UB_DICT(name, type)                                                   \
	name {struct UBrbnode node; char* key; type val;}

#define ub_dict_setkey(dict, str) ((dict)->key = (str))

#define ub_dict_getkey(dict) ((dict)->key)

#define ub_dict_cast(dict) (&(dict)->val)

#define ub_dict_ins(dict, entry)                                              \
	({                                                                     \
		__typeof__(entry) _entry = (entry);                            \
		const char* _key = _entry->key;                                \
		struct UBrbnode* _root = &(dict)->node;                        \
		struct UBrbnode** _i = &_root;                                 \
		struct UBrbnode* _p = NULL;                                    \
		struct UBrbnode* _node = &_entry->node;                        \
									       \
		while (*_i) {                                                  \
			_p = *_i;                                              \
			if (strcmp(_key, ((__typeof__(dict))_p)->key) < 0)     \
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
		int _tmp = 0;                                                  \
									       \
		while (_dict) {                                                \
			_tmp = strcmp(_key, _dict->key);                       \
			if (_tmp < 0)                                          \
				_dict = (__typeof__(_dict))&_dict->node.left;  \
			else if (_tmp > 0)                                     \
				_dict = (__typeof__(_dict))&_dict->node.right; \
			else                                                   \
				break;                                         \
		}                                                              \
		_dict;                                                         \
	})

#endif /* UB_DICT_H */
