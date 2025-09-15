#ifndef PUBSUB_H
#define PUBSUB_H

#include "sturk/broker.h"
#include "sturk/str.h"

extern const struct StMessageVt SAMPLE_MESSAGE_API[];

struct StStrQ* single_thread_pubsub(void);

struct StStrBag* multi_thread_pubsub(void);

#endif /* PUBSUB_H */
