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

#define ub_binode_ins(root, entry, pos) _ub_binode_ins(root, entry, pos)

#define ub_binode_rem(rootp, pos) _ub_binode_rem(rootp, pos)

#define ub_cirq_cast(cirq)                                                    \
	({                                                                     \
		__typeof__(cirq) _mcirq = (cirq);                              \
									       \
		ub_ensure(_mcirq, "Null pointer.");                            \
		&_mcirq->node;                                                 \
	})

#define ub_cirq_data(cirq)                                                    \
	({                                                                     \
		__typeof__(cirq) _mcirq = (cirq);                              \
									       \
		ub_ensure(_mcirq, "Null pointer.");                            \
		&_mcirq->data;                                                 \
	})

#define ub_cirq_cont(ptr, type) ub_container_of(ptr, type, node)

#define ub_cirq_ins(cirq, ...) _UB_CIRQ_INS((cirq), __VA_ARGS__, -1)

#define ub_cirq_rem(...) _UB_CIRQ_REM(__VA_ARGS__, 0)

struct UBinode*
_ub_binode_ins(struct UBinode* root, struct UBinode* entry, int pos, ...);

struct UBinode* _ub_binode_rem(struct UBinode** rootp, int pos, ...);

#define _UB_CIRQ_INS(cirq, entry, pos, ...)                                   \
	({                                                                     \
		__typeof__(cirq) _cirq = (cirq);                               \
									       \
		ub_cirq_cont(                                                  \
			_ub_binode_ins(                                        \
				_cirq ? &_cirq->node : NULL,                   \
				ub_cirq_cast(entry), pos, __VA_ARGS__          \
			),                                                     \
			__typeof__(*cirq)                                      \
		);                                                             \
	})

#define _UB_CIRQ_REM(cirqp, pos, ...)                                         \
	({                                                                     \
		struct UBinode* node = ub_cirq_cast(*cirqp);                   \
									       \
		ub_cirq_cont(                                                  \
			_ub_binode_rem(&node, __VA_ARGS__, 0),                 \
			__typeof__(**cirqp)                                    \
		);                                                             \
	})

#endif /* UB_CIRQ_H */
