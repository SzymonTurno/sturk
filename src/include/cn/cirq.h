#ifndef CN_CIRQ_H
#define CN_CIRQ_H

#include "cn/arith.h"
#include "cn/logger/except.h"

struct CnBinode {
	struct CnBinode* next;
	struct CnBinode* prev;
};

#define CN_CIRQ(name, type) name { struct CnBinode node; type data; }

struct CnBinode* cn_binode_sibl(struct CnBinode* node, int pos);

struct CnBinode* cn_binode_ins(struct CnBinode* root, struct CnBinode* entry,
	int pos);

struct CnBinode* cn_binode_rem(struct CnBinode** rootp, int pos);

#ifdef __STRICT_ANSI__

#define cn_cirq_cast(cirq) (&(cirq)->node)

#define cn_cirq_data(cirq) (&(cirq)->data)

#define cn_cirq_from(ptr, type) ((type*)(ptr))

#else /* __STRICT_ANSI__ */

#define cn_cirq_cast(cirq)                                                    \
	({                                                                     \
		__typeof__(cirq) _cirq = (cirq);                               \
									       \
		CN_ENSURE(_cirq, CN_ECODES.null_param);                        \
		&_cirq->node;                                                  \
	})

#define cn_cirq_data(cirq)                                                    \
	({                                                                     \
		__typeof__(cirq) _cirq = (cirq);                               \
									       \
		CN_ENSURE(_cirq, CN_ECODES.null_param);                        \
		&_cirq->data;                                                  \
	})

#define cn_cirq_from(ptr, type) cn_container_of(ptr, type, node)

#endif /* __STRICT_ANSI__ */

#define cn_cirq_ins(cirq, ...) _CN_CIRQ_INS((cirq), __VA_ARGS__, -1,)

#define cn_cirq_rem(...) _CN_CIRQ_REM(__VA_ARGS__, 0,)

#ifdef __STRICT_ANSI__

#define _CN_CIRQ_INS(cirq, entry, pos, ...)                                   \
	((void*)cn_binode_ins((struct CnBinode*)(cirq), &(entry)->node, pos))

#define _CN_CIRQ_REM(cirqp, pos, ...)                                         \
	((void*)cn_binode_rem((struct CnBinode**)(cirqp), pos))

#else /* __STRICT_ANSI__ */

#define _CN_CIRQ_INS(cirq, entry, pos, ...)                                   \
	({                                                                     \
		__typeof__(entry) _cirq = (cirq);                              \
		__typeof__(entry) _entry = (entry);                            \
									       \
		cn_cirq_from(                                                  \
			cn_binode_ins(_cirq ? &_cirq->node : NULL,             \
				_entry ? &_entry->node : NULL, (pos)),         \
			__typeof__(*entry));                                   \
	})

#define _CN_CIRQ_REM(cirqp, pos, ...)                                         \
	({                                                                     \
		__typeof__(cirqp) _cirqp = (cirqp);                            \
		struct CnBinode* _node = _cirqp ? &(*_cirqp)->node : NULL;     \
		__typeof__(*cirqp) _ret =                                      \
			cn_cirq_from(                                          \
				cn_binode_rem(&_node, (pos)),                  \
				__typeof__(**_cirqp));                         \
									       \
		*_cirqp = cn_cirq_from(_node, __typeof__(**_cirqp));           \
		_ret;                                                          \
	})

#endif /* __STRICT_ANSI__ */

#endif /* CN_CIRQ_H */
