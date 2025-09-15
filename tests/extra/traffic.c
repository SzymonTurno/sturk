#include "simpte.h"
#include "sturk/broker.h"
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>

#define AWAIT_AND_ASSERT_TOPIC(sber, expected_topic)                           \
	TEST_ASSERT_EQUAL_STRING(                                              \
		expected_topic,                                                \
		channel_gettopic(msg_getchan(subscriber_await(sber))))

#define N_THREADS 20
#define N_NOTIFS  100

static int counters[N_THREADS][N_THREADS];
static StBroker* broker;
static const char* topics1[] = {"Lorem", "ipsum", "aliqua.Ut"};
static const char* topics2[] = {"veniam, quis", "consequat.Duis", "laboris"};

SIMPTE_GROUP(broker_extra, "broker_extra");

struct Message {
	StChannel* channel;
	char payload[8];
};

static size_t size(void)
{
	return sizeof(struct Message);
}

static void init(void* msg, va_list vlist)
{
	struct Message* tmp = msg;
	char* format = NULL;

	tmp->channel = va_arg(vlist, StChannel*);
	format = va_arg(vlist, char*);
	vsnprintf(tmp->payload, 8, format, vlist);
}

static void deinit(void* msg)
{
	(void)msg;
}

static StChannel* msg_getchan(void* msg)
{
	return ((struct Message*)msg)->channel;
}

static int msg_getid(void* msg)
{
	return atoi(((struct Message*)msg)->payload);
}

const struct StMessageVt MESSAGE_API[] = {
	{.size = size, .ctor = init, .dtor = deinit}};

static void* task(void* arg)
{
	const intptr_t id = (intptr_t)arg;
	StSubscriber* sber = subscriber_create(broker);
	struct Message* msg = NULL;
	const char** topics_in = NULL;
	const char** topics_out = NULL;
	int n_in = 0;
	int n_out = 0;
	int i = 0;
	int done = 0;
	StChannel* ch = NULL;

	if ((id % 2)) {
		topics_in = topics1;
		n_in = sizeof(topics1) / sizeof(topics1[0]);
		topics_out = topics2;
		n_out = sizeof(topics2) / sizeof(topics2[0]);
	} else {
		topics_in = topics2;
		n_in = sizeof(topics2) / sizeof(topics2[0]);
		topics_out = topics1;
		n_out = sizeof(topics1) / sizeof(topics1[0]);
	}
	subscribe(sber, "request.traffic");
	subscribe(sber, "request.join");
	for (int i = 0; i < n_in; i++)
		subscribe(sber, topics_in[i]);
	ch = broker_search(broker, "response.subscribed");
	publish(ch, ch, "%d", id);

	AWAIT_AND_ASSERT_TOPIC(sber, "request.traffic");
	for (;;)
		if (msg || (msg = subscriber_poll(sber))) {
			if (strcmp("request.join",
			           channel_gettopic(msg_getchan(msg))) == 0)
				break;
			TEST_ASSERT_GREATER_THAN_INT(
				N_THREADS, msg_getid(msg));
			TEST_ASSERT_LESS_OR_EQUAL_INT(0, msg_getid(msg));
			counters[id][msg_getid(msg)]++;
			msg = NULL;
		} else if (i < N_NOTIFS) {
			ch = broker_search(broker, topics_out[rand() % n_out]);
			publish(ch, ch, "%d", id);
			++i;
		} else if (!done) {
			printf("[%ld] Publishing to \"response.done\".\n", id);
			done = 1;
			ch = broker_search(broker, "response.done");
			publish(ch, ch, "%ld", id);
		} else {
			msg = subscriber_await(sber);
		}
	ch = broker_search(broker, "response.join");
	publish(ch, ch, "%ld", id);
	return NULL;
}

TEST(broker_extra, should_be_safe_from_race_conditions)
{
	StSubscriber* sber = NULL;
	pthread_t thid[N_THREADS];
	pthread_attr_t attr;
	StChannel* ch = NULL;

	pthread_attr_init(&attr);
	pthread_attr_setstacksize(&attr, 512);
	broker = broker_create(MESSAGE_API);
	sber = subscriber_create(broker);
	subscribe(sber, "response.subscribed");
	subscribe(sber, "response.done");
	subscribe(sber, "response.join");
	for (intptr_t i = 0; i < N_THREADS; i++)
		pthread_create(&thid[i], &attr, task, (void*)i);

	for (int i = 0; i < N_THREADS; i++)
		AWAIT_AND_ASSERT_TOPIC(sber, "response.subscribed");
	ch = broker_search(broker, "request.traffic");
	publish(ch, ch, "%p", NULL);

	for (int i = 0; i < N_THREADS; i++)
		AWAIT_AND_ASSERT_TOPIC(sber, "response.done");
	ch = broker_search(broker, "request.join");
	publish(ch, ch, "%p", NULL);

	for (int i = 0; i < N_THREADS; i++)
		pthread_join(thid[msg_getid(subscriber_await(sber))], NULL);

	for (int i = 0; i < N_THREADS; i++)
		for (int j = !(i % 2); j < N_THREADS; j += 2)
			TEST_ASSERT_EQUAL_INT(N_NOTIFS, counters[i][j]);
	broker_destroy(broker);
}

void run_broker_extra_tests(void)
{
	RUN_TEST_CASE(broker_extra, should_be_safe_from_race_conditions);
}
