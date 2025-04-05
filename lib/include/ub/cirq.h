#ifndef UB_CIRQ_H
#define UB_CIRQ_H

#include "ub/arith.h"
#include "ub/debug.h"

struct UBinode {
	struct UBinode* next;
	struct UBinode* prev;
};

#define UB_CIRQ(name, type) name { struct UBinode node; type data; }

struct UBinode* ub_binode_sibl(struct UBinode* node, int pos);

struct UBinode* ub_binode_ins(struct UBinode* root, struct UBinode* entry,
	int pos);

struct UBinode* ub_binode_rem(struct UBinode** rootp, int pos);

#define ub_cirq_cast(cirq)                                                    \
	({                                                                     \
		__typeof__(cirq) _cirq = (cirq);                               \
									       \
		ub_ensure(_cirq, "Null pointer.");                             \
		&_cirq->node;                                                  \
	})

#define ub_cirq_data(cirq)                                                    \
	({                                                                     \
		__typeof__(cirq) _cirq = (cirq);                               \
									       \
		ub_ensure(_cirq, "Null pointer.");                             \
		&_cirq->data;                                                  \
	})

#define ub_cirq_cont(ptr, type) ub_container_of(ptr, type, node)

#define ub_cirq_ins(cirq, ...) _UB_CIRQ_INS((cirq), __VA_ARGS__, -1)

#define ub_cirq_rem(...) _UB_CIRQ_REM(__VA_ARGS__, 0)

#define _UB_CIRQ_INS(cirq, entry, pos, ...)                                   \
	({                                                                     \
		__typeof__(entry) _cirq = (cirq);                              \
		__typeof__(entry) _entry = (entry);                            \
									       \
		ub_cirq_cont(                                                  \
			ub_binode_ins(_cirq ? &_cirq->node : NULL,             \
				_entry ? &_entry->node : NULL, (pos)),         \
			__typeof__(*entry));                                   \
	})

#define _UB_CIRQ_REM(cirqp, pos, ...)                                         \
	({                                                                     \
		__typeof__(cirqp) _cirqp = (cirqp);                            \
		struct UBinode* _node = _cirqp ? &(*_cirqp)->node : NULL;      \
		__typeof__(*cirqp) _ret =                                      \
			ub_cirq_cont(                                          \
				ub_binode_rem(&_node, (pos)),                  \
				__typeof__(**_cirqp));                         \
									       \
		*_cirqp = ub_cirq_cont(_node, __typeof__(**_cirqp));           \
		_ret;                                                          \
	})

#endif /* UB_CIRQ_H */
