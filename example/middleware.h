#ifndef MIDDLEWARE_H
#define MIDDLEWARE_H

#include "ub/mom/scribe.h"

struct Payload {
	int new;
	int old;
};

struct Observer {
	void* instance;
	void (*notifior)(void*, struct Payload*);
};

static inline UBscriber* create_subscriber(void* instance,
	void (*notifior)(void*, struct Payload*))
{
	struct Observer obs = {instance, notifior};

	return ub_scriber_create(&obs, sizeof(obs));
}

static inline size_t get_payload_size(void)
{
	return sizeof(struct Payload);
}

static inline UBload init_payload(UBload load, va_list vlist)
{
	((struct Payload*)load)->new = va_arg(vlist, int);
	return load;
}

static inline void eat_payload(void* eater, UBload load)
{
	struct Observer* p = eater;

	p->notifior(p->instance, load);
}

static const struct UBloadVt PAYLOAD[] = {{
	.size = get_payload_size,
	.ctor = init_payload,
	.eat = eat_payload
}};

#endif /* MIDDLEWARE_H */
