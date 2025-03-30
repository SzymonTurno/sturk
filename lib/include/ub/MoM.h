#ifndef UB_MOM_H
#define UB_MOM_H

#include <stddef.h>
#include <stdarg.h>

typedef char UBload;

struct UBloadVt {
	size_t (*size)(void);
	void (*ctor)(UBload*, va_list);
	void (*dtor)(UBload*);
};

typedef struct UBroker UBroker;

typedef struct UBscriber UBscriber;

typedef struct UBchan UBchan;

UBroker* ub_broker_create(const struct UBloadVt* vp);

void ub_broker_destroy(UBroker* broker);

UBchan* ub_broker_search(UBroker* broker, const char* topic);

UBscriber* ub_scriber_create(UBroker* broker);

void ub_scriber_destroy(UBscriber* scriber);

UBload* ub_scriber_await(UBscriber* scriber, UBchan** chan);

void ub_scriber_release(UBscriber* scriber);

const char* ub_get_topic(UBchan* chan);

void ub_lish(UBchan* chan, ...);

void ub_scribe(UBscriber* scriber, const char* topic);

#endif /* UB_MOM_H */
