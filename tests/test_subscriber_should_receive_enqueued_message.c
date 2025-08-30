/*
 * File included as an example in the documentation.
 *
 * Note, line numbering in this file must be coherent with the documentation.
 */

TEST(subscriber, should_receive_enqueued_message)
{
	StBroker* broker = broker_create(SAMPLE_LOAD_API);
	StSubscriber* sber = subscriber_create(broker);
	StLoad* load = NULL;

	subscribe(sber, "test");
	publish(broker_search(broker, "test"), "%X", 0xF00D);
	TEST_ASSERT_NULL(channel_gettopic(load_getchan(load)));
	load = subscriber_await(sber);
	TEST_ASSERT_EQUAL_STRING("F00D", load);
	TEST_ASSERT_EQUAL_STRING("test", channel_gettopic(load_getchan(load)));
	broker_destroy(broker);
}
