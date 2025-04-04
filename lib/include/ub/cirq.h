#ifndef UB_CIRQ_H
#define UB_CIRQ_H

#include "ub/arith.h"
#include "ub/debug.h"

struct UBinode {
	struct UBinode* next;
	struct UBinode* prev;
};

struct UBinode* ub_binode_sibl(struct UBinode* node, int pos);

struct UBinode*
_ub_binode_ins(struct UBinode* root, struct UBinode* entry, int pos, ...);

struct UBinode* _ub_binode_rem(struct UBinode** nodep, int pos, ...);

#define ub_binode_ins(root, ...) _ub_binode_ins(root, __VA_ARGS__, -1)

#define ub_binode_rem(...) _ub_binode_rem(__VA_ARGS__, 0)

#define UB_CIRQ(name, type) name {struct UBinode node; type data;}

#define ub_cirq_cast(cirq)                                                    \
	({                                                                     \
		__typeof__(cirq) _mcirq = (cirq);                              \
									       \
		ub_ensure(_mcirq, "Bad pointer.");                             \
		&(_mcirq)->node;                                               \
	})

#define ub_cirq_data(cirq)                                                    \
	({                                                                     \
		__typeof__(cirq) _mcirq = (cirq);                              \
									       \
		ub_ensure(_mcirq, "Bad pointer.");                             \
		&(_mcirq)->data;                                               \
	})

#define ub_cirq_cont(ptr, type) ub_container_of(ptr, type, node)

#define ub_cirq_ins(cirq, ...) _UB_CIRQ_INS((cirq), __VA_ARGS__, -1)

#define ub_cirq_rem(...) _UB_CIRQ_REM(__VA_ARGS__, 0)

#define ub_cirq_sibl(cirq, pos) ub_binode_sibl(ub_cirq_cast(cirq), pos)

#define _UB_CIRQ_INS(cirq, entry, pos, ...)                                   \
	({                                                                     \
		__typeof__(cirq) _cirq = (cirq);                               \
									       \
		_ub_binode_ins(_cirq ? ub_cirq_cast(_cirq) : NULL,             \
			ub_cirq_cast(entry), pos, __VA_ARGS__);                \
		_cirq;                                                         \
	})

#define _UB_CIRQ_REM(cirqp, pos, ...)                                         \
	({                                                                     \
		struct UBinode* node = ub_cirq_cast(*cirqp);                   \
									       \
		node = _ub_binode_rem(&node, __VA_ARGS__, 0);                  \
		ub_cirq_cont(node, __typeof__(cirqp));                         \
	})

#endif /* UB_CIRQ_H */
