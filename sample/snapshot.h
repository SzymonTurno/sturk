#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include "ub/os/sysio.h"

void snapshot_init(const char* factual);

void snapshot_deinit(void);

UBfstream* snapshot_get_stream(void);

int snapshot_test_ordered(const char* fexpected);

int snapshot_test_unordered(const char* fexpected);

#endif /* SNAPSHOT_H */
