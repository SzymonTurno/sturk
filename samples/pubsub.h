#ifndef PUBSUB_H
#define PUBSUB_H

#include "sturk/broker.h"
#include "sturk/str.h"

extern const struct StLoadVt SAMPLE_LOAD_API[];

struct StStrq* single_thread_pubsub(void);

struct StStrbag* multi_thread_pubsub(void);

#endif /* PUBSUB_H */
