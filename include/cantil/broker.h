#ifndef CANTIL_BROKER_H
#define CANTIL_BROKER_H

#include "cn/broker.h"

#define broker_create cn_broker_create

#define broker_destroy cn_broker_destroy

#define broker_search cn_broker_search

#define subscriber_create cn_subscriber_create

#define subscriber_destroy cn_subscriber_destroy

#define subscriber_await cn_subscriber_await

#define subscriber_poll cn_subscriber_poll

#define subscriber_release cn_subscriber_release

#define channel_gettopic cn_channel_gettopic

#define publish cn_publish

#define subscribe cn_subscribe

#endif /* CANTIL_BROKER_H */
