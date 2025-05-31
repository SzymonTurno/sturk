#ifndef PUBSUB_H
#define PUBSUB_H

#include "ub/str.h"

struct CyStrq* single_thread_pubsub(void);

struct CyStrbag* multi_thread_pubsub(void);

#endif /* PUBSUB_H */
