#ifndef UNITTEST_H
#define UNITTEST_H

#include "unity.h"
#include "unity_fixture.h"
#include "snapshot.h"
#include "UB/logger/log.h"

#define TEST_ASSERT_SNAPSHOT_ORDERED(fexpected)                               \
	TEST_ASSERT_EQUAL(0, snapshot_test_ordered(fexpected));

#define TEST_ASSERT_SNAPSHOT_UNORDERED(fexpected)                             \
	TEST_ASSERT_EQUAL(0, snapshot_test_unordered(fexpected));

#define TEST_ASSERT_EQUAL_STREAM_GETLINE(stream, str) NOT_IMPLEMENTED

#define TEST_ASSERT_STREAM_CONTAINS(stream, str) NOT_IMPLEMENTED

#endif /* UNITTEST_H */
