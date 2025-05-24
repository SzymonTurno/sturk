#ifndef UB_SNAPSHOT_H
#define UB_SNAPSHOT_H

#include "ub/os/sysio.h"

typedef struct UBsnapshot UBsnapshot;

UBsnapshot* ub_snapshot_open(const char* factual);

UBfstream* ub_snapshot_cast(UBsnapshot* snapshot);

int ub_snapshot_ordered(UBsnapshot* snapshot, const char* fexpected);

int ub_snapshot_unordered(UBsnapshot* snapshot, const char* fexpected);

void ub_snapshot_close(UBsnapshot* snapshot);

#endif /* UB_SNAPSHOT_H */
