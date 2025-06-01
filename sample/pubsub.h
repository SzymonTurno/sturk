#ifndef PUBSUB_H
#define PUBSUB_H

#include "cn/str.h"

struct CnStrq* single_thread_pubsub(void);

struct CnStrbag* multi_thread_pubsub(void);

#endif /* PUBSUB_H */
