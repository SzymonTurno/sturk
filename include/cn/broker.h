#ifndef CN_BROKER_H
#define CN_BROKER_H

#include <stdarg.h>
#include <stddef.h>

typedef char CnLoad;

struct CnLoadVt {
	size_t (*size)(void);
	void (*ctor)(CnLoad*, va_list);
	void (*dtor)(CnLoad*);
};

typedef struct CnBroker CnBroker;

typedef struct CnSubscriber CnSubscriber;

typedef struct CnChannel CnChannel;

void cn_publish(CnChannel* ch, ...);

void cn_subscribe(CnSubscriber* sber, const char* topic);

CnBroker* cn_broker_create(const struct CnLoadVt* vp);

void cn_broker_destroy(CnBroker* broker);

CnChannel* cn_broker_search(CnBroker* broker, const char* topic);

const char* cn_channel_gettopic(CnChannel* ch);

CnSubscriber* cn_subscriber_create(CnBroker* broker);

void cn_subscriber_destroy(CnSubscriber* sber);

CnLoad* cn_subscriber_await(CnSubscriber* sber, CnChannel** ch);

CnLoad* cn_subscriber_poll(CnSubscriber* sber, CnChannel** ch);

void cn_subscriber_release(CnSubscriber* sber);

#endif /* CN_BROKER_H */
