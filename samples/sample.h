#ifndef PUBSUB_H
#define PUBSUB_H

#include "sturk/io/buffer.h"
#include "sturk/str.h"

extern const struct StIoVt SAMPLE_FILE_API[];

struct StStrQ* single_thread_pubsub(void);

struct StStrBag* multi_thread_pubsub(void);

#endif /* PUBSUB_H */
