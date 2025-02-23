#ifndef UB_CIRQ_H
#define UB_CIRQ_H

#define UB_CIRQ(name, type) name {name* next; name* prev; type data;}

#define ub_cirq_cast(cirq) (&(cirq)->data)

#define ub_cirq_ins(cirq, ...) _UB_CIRQ_INS((cirq), __VA_ARGS__, -1,)

#define ub_cirq_rem(...) _UB_CIRQ_REM(__VA_ARGS__, 0,)

#define ub_cirq_sibl(cirq, pos)                                               \
	({                                                                     \
		__typeof__(cirq) _cirq_ = (cirq);                              \
		__typeof__(pos) _pos_ = (pos);                                 \
									       \
		while (_pos_-- > 0)                                            \
			_cirq_ = _cirq_->next;                                 \
		while (_pos_++ < 0)                                            \
			_cirq_ = _cirq_->prev;                                 \
		_cirq_;                                                        \
	})

#define _UB_CIRQ_INS(cirq, entry, pos, ...)                                   \
	({                                                                     \
		__typeof__(cirq) _cirq = (cirq);                               \
		__typeof__(entry) _entry = (entry);                            \
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
		__typeof__(*_cirqp) _entry = ub_cirq_sibl(*cirqp, _pos);       \
									       \
		_entry->next->prev = _entry->prev;                             \
		_entry->prev->next = _entry->next;                             \
		if (_pos == 0)                                                 \
			*_cirqp = _entry->next;                                \
		_entry;                                                        \
	})

#endif /* UB_CIRQ_H */
