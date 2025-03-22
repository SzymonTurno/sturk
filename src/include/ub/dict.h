#ifndef UB_DICT_H
#define UB_DICT_H

#include "ub/rbtree.h"
#include "ub/arith.h"
#include <string.h>

#define UB_DICT(name, type)                                                   \
	name {struct UBrbnode _dictn_; char* _key_; type _val_;}

#define ub_dict_cast(dict) (&(dict)->_dictn_)

#define ub_dict_setkey(dict, str) ((dict)->_key_ = (str))

#define ub_dict_getkey(dict) ((dict)->_key_)

#define ub_dict_data(dict) (&(dict)->_val_)

#define ub_dict_container(ptr, type) ub_container_of(ptr, type, _dictn_)

#define ub_dict_ins(dict, entry)                                              \
	({                                                                     \
		__typeof__(entry) _entry = (entry);                            \
		const char* _key = _entry->_key_;                              \
		struct UBrbnode* _root = &(dict)->_dictn_;                     \
		struct UBrbnode** _i = &_root;                                 \
		struct UBrbnode* _p = NULL;                                    \
		struct UBrbnode* _node = &_entry->_dictn_;                     \
									       \
		while (*_i) {                                                  \
			_p = *_i;                                              \
			if (strcmp(_key, ((__typeof__(dict))_p)->_key_) < 0)   \
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
		while (_dict) {                                                \
			_tmp = strcmp(_key, _dict->_key_);                     \
			if (_tmp < 0) {                                        \
				_node = _dict->_dictn_.left;                   \
				_dict = (__typeof__(_dict))_node;              \
			} else if (_tmp > 0) {                                 \
				_node = _dict->_dictn_.right;                  \
				_dict = (__typeof__(_dict))_node;              \
			} else {                                               \
				break;                                         \
			}                                                      \
		}                                                              \
		_dict;                                                         \
	})

#endif /* UB_DICT_H */
