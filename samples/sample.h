#ifndef PUBSUB_H
#define PUBSUB_H

#include "sturk/io/api.h"
#include "sturk/str.h"
#include <stdio.h>

struct StIo* iofile_create(FILE* fp);

struct StStrQ* single_thread_pubsub(void);

struct StStrBag* multi_thread_pubsub(void);

#endif /* PUBSUB_H */
