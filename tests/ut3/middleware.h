#ifndef MIDDLEWARE_H
#define MIDDLEWARE_H

#include "UB/broker.h"

struct Payload {
	int new;
	int old;
};

struct Publisher {
	UBroker* broker;
	union {
		int data;
		void* align;
	} u;
};

struct Subscriber {
	UBscriber* scriber;
	struct Payload* pl;
	UBchan* chan;
};

extern const struct UBloadVt PAYLOAD[];

int receive(struct Subscriber* sub);

void publish(struct Publisher* pub, const char* topic, int data);

#endif /* MIDDLEWARE_H */
