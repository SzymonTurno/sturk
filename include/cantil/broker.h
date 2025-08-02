/**
 * @file cantil/broker.h
 *
 * @see cn/broker.h.
 */

#ifndef CANTIL_BROKER_H
#define CANTIL_BROKER_H

#include "cn/broker.h"

/** @see cn_publish() */
#define publish cn_publish

/** @see cn_subscribe() */
#define subscribe cn_subscribe

/** @see cn_broker_create() */
#define broker_create cn_broker_create

/** @see cn_broker_destroy() */
#define broker_destroy cn_broker_destroy

/** @see cn_broker_search() */
#define broker_search cn_broker_search

/** @see cn_channel_gettopic() */
#define channel_gettopic cn_channel_gettopic

/** @see cn_subscriber_create() */
#define subscriber_create cn_subscriber_create

/** @see cn_subscriber_destroy() */
#define subscriber_destroy cn_subscriber_destroy

/** @see cn_subscriber_await() */
#define subscriber_await cn_subscriber_await

/** @see cn_subscriber_poll() */
#define subscriber_poll cn_subscriber_poll

/** @see cn_subscriber_release() */
#define subscriber_release cn_subscriber_release

#endif /* CANTIL_BROKER_H */
