#ifndef UB_CIRQ_H
#define UB_CIRQ_H

#include "ub/arith.h"
#include "ub/debug.h"

struct UBcirq {
	union {
		struct {
			struct UBcirq* next;
			struct UBcirq* prev;
		};
		struct {
			struct UBcirq* next;
			struct UBcirq* prev;
		} _cirqn_;
	};
};

#define UB_CIRQ(name, type) name {struct UBcirq _cirqn_; type _data_;}

#define ub_cirq_cast(cirq)                                                    \
	({                                                                     \
		__typeof__(cirq) _mcirq = (cirq);                              \
									       \
		ub_ensure(_mcirq, "Bad pointer.");                             \
		(struct UBcirq*)(&(_mcirq)->_cirqn_);                          \
	})

#define ub_cirq_data(cirq)                                                    \
	({                                                                     \
		__typeof__(cirq) _mcirq = (cirq);                              \
									       \
		ub_ensure(_mcirq, "Bad pointer.");                             \
		(&(_mcirq)->_data_);                                           \
	})

#define ub_cirq_cont(ptr, type) ub_container_of(ptr, type, _cirqn_)

#define ub_cirq_ins(cirq, ...) _UB_CIRQ_INS((cirq), __VA_ARGS__, -1,)

#define ub_cirq_rem(...) _UB_CIRQ_REM(__VA_ARGS__, 0,)

#define ub_cirq_sibl(cirq, pos)                                               \
	({                                                                     \
		struct UBcirq* _cirq_ = ub_cirq_cast(cirq);                    \
		long long _pos_ = (pos);                                       \
									       \
		while (_pos_ > 0) {                                            \
			_cirq_ = _cirq_->next;                                 \
			--_pos_;                                               \
		}                                                              \
									       \
		while (_pos < 0) {                                             \
			_cirq_ = _cirq_->prev;                                 \
			++_pos_;                                               \
		}                                                              \
		_cirq_;                                                        \
	})

#define _UB_CIRQ_INS(cirq, entry, pos, ...)                                   \
	({                                                                     \
		__typeof__(cirq) _tmp = (cirq);                                \
		struct UBcirq* _cirq = _tmp ? ub_cirq_cast(_tmp) : NULL;       \
		struct UBcirq* _entry = ub_cirq_cast(entry);                   \
		long long _pos = (pos);                                        \
									       \
		if (_cirq) {                                                   \
			if (_pos > 0)                                          \
				_cirq = ub_cirq_sibl(cirq, _pos);              \
			else if (_pos < -1)                                    \
				_cirq = ub_cirq_sibl(cirq, _pos + 1);          \
			_entry->next = _cirq;                                  \
			_entry->prev = _cirq->prev;                            \
			_cirq->prev = _entry;                                  \
			_entry->prev->next = _entry;                           \
			if (_pos == 0)                                         \
				_cirq = entry;                                 \
		} else {                                                       \
			_entry->next = _entry;                                 \
			_entry->prev = _entry;                                 \
			_cirq = _entry;                                        \
		}                                                              \
		_cirq;                                                         \
	})

#define _UB_CIRQ_REM(cirqp, pos, ...)                                         \
	({                                                                     \
		__typeof__(cirqp) _cirqp = (cirqp);                            \
		long long _pos = (pos);                                        \
		struct UBcirq* _first = NULL;                                  \
		struct UBcirq* _entry = NULL;                                  \
									       \
		if (*_cirqp) {                                                 \
			      _first = ub_cirq_cast(*_cirqp);                  \
			      _entry = ub_cirq_sibl(_first, _pos);             \
			      _entry->next->prev = _entry->prev;               \
			      _entry->prev->next = _entry->next;               \
			      if (_entry == _entry->next)                      \
				      *_cirqp = NULL;                          \
			      else if (_entry == _first)                       \
				      *(struct UBcirq**)_cirqp = _entry->next; \
		}                                                              \
		_entry;                                                        \
	})

#endif /* UB_CIRQ_H */
