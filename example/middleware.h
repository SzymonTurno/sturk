#ifndef MIDDLEWARE_H
#define MIDDLEWARE_H

#include "ub/mom.h"

struct Payload {
	int new;
	int old;
};

struct Subscriber {
	UBscriber* scriber;
	struct Payload* pl;
	UBchan* chan;
};

extern const struct UBloadVt PAYLOAD[];

void receive(struct Subscriber* sub);

#endif /* MIDDLEWARE_H */
