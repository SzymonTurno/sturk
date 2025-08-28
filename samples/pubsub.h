#ifndef PUBSUB_H
#define PUBSUB_H

#include "sturk/broker.h"
#include "sturk/str.h"

extern const struct CnLoadVt SAMPLE_LOAD_API[];

struct CnStrq* single_thread_pubsub(void);

struct CnStrbag* multi_thread_pubsub(void);

#endif /* PUBSUB_H */
