#include "snapshot.h"
#include "UB/debug/snapshot.h"
#include "UB/os/mutex.h"

static UBsnapshot* snapshot;
static UBmutex* mutex;

void snapshot_init(const char* factual)
{
	if (!mutex)
		mutex = mutex_create(MUTEX_POLICY_PRIO_INHERIT);
	mutex_lock(mutex);
	if (!snapshot)
		snapshot = snapshot_open(factual);
	mutex_unlock(mutex);
}

void snapshot_deinit(void)
{
	mutex_lock(mutex);
	snapshot_close(snapshot);
	snapshot = NULL;
	mutex_unlock(mutex);
	mutex_destroy(mutex);
	mutex = NULL;
}

UBfstream* snapshot_get_stream(void)
{
	UBfstream* ret = NULL;

	mutex_lock(mutex);
	ret = snapshot_cast(snapshot);
	mutex_unlock(mutex);
	return ret;
}

int snapshot_test_ordered(const char* fexpected)
{
	int ret = 0;

	mutex_lock(mutex);
	ret = snapshot_ordered(snapshot, fexpected);
	mutex_unlock(mutex);
	return ret;
}

int snapshot_test_unordered(const char* fexpected)
{
	int ret = 0;

	mutex_lock(mutex);
	ret = snapshot_ordered(snapshot, fexpected);
	mutex_unlock(mutex);
	return ret;
}
