#include "simpte.h"
#include "sturk/broker.h"
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>

#define AWAIT_AND_ASSERT_TOPIC(sber, expected_topic)                           \
	TEST_ASSERT_EQUAL_STRING(expected_topic, await_and_gettopic(sber))

#define N_THREADS 20
#define N_NOTIFS  100

static int counters[N_THREADS][N_THREADS];
static StBroker* broker;
static const char* topics1[] = {"Lorem", "ipsum", "aliqua.Ut"};
static const char* topics2[] = {"veniam, quis", "consequat.Duis", "laboris"};

struct Payload {
	char info[8];
};

static int msg_getid(struct Payload* pload)
{
	return atoi(pload->info);
}

static const char* await_and_gettopic(StSubscriber* sber)
{
	struct StMessage msg = subscriber_await(sber);

	return channel_gettopic(message_getchannel(msg));
}

static void publ(StChannel* ch, const char* fmt, ...)
{
	va_list va;
	struct StMessage msg = channel_allocmsg(ch);
	struct Payload* pload = msg.payload;

	va_start(va, fmt);
	vsnprintf(pload->info, 8, fmt, va);
	va_end(va);
	publish(&msg);
}

static void* task(void* arg)
{
	const intptr_t id = (intptr_t)arg;
	StSubscriber* sber = subscriber_create(broker);
	struct StMessage msg = {.payload = NULL};
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
	publ(ch, "%d", id);

	AWAIT_AND_ASSERT_TOPIC(sber, "request.traffic");
	for (;;)
		if (msg.payload || (msg = subscriber_poll(sber)).payload) {
			if (strcmp("request.join",
			           channel_gettopic(message_getchannel(msg))) ==
			    0)
				break;
			TEST_ASSERT_GREATER_THAN_INT(
				N_THREADS, msg_getid(msg.payload));
			TEST_ASSERT_LESS_OR_EQUAL_INT(
				0, msg_getid(msg.payload));
			counters[id][msg_getid(msg.payload)]++;
			msg.payload = NULL;
		} else if (i < N_NOTIFS) {
			ch = broker_search(broker, topics_out[rand() % n_out]);
			publ(ch, "%d", id);
			++i;
		} else if (!done) {
			printf("[%ld] Publishing to \"response.done\".\n", id);
			done = 1;
			ch = broker_search(broker, "response.done");
			publ(ch, "%ld", id);
		} else {
			msg = subscriber_await(sber);
		}
	ch = broker_search(broker, "response.join");
	publ(ch, "%ld", id);
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
	broker = broker_create(sizeof(struct Payload));
	sber = subscriber_create(broker);
	subscribe(sber, "response.subscribed");
	subscribe(sber, "response.done");
	subscribe(sber, "response.join");
	for (intptr_t i = 0; i < N_THREADS; i++)
		pthread_create(&thid[i], &attr, task, (void*)i);

	for (int i = 0; i < N_THREADS; i++)
		AWAIT_AND_ASSERT_TOPIC(sber, "response.subscribed");
	ch = broker_search(broker, "request.traffic");
	publ(ch, "%p", NULL);

	for (int i = 0; i < N_THREADS; i++)
		AWAIT_AND_ASSERT_TOPIC(sber, "response.done");
	ch = broker_search(broker, "request.join");
	publ(ch, "%p", NULL);

	for (int i = 0; i < N_THREADS; i++)
		pthread_join(
			thid[msg_getid(subscriber_await(sber).payload)], NULL);

	for (int i = 0; i < N_THREADS; i++)
		for (int j = !(i % 2); j < N_THREADS; j += 2)
			TEST_ASSERT_EQUAL_INT(N_NOTIFS, counters[i][j]);
	broker_destroy(broker);
}

void run_traffic_tests(void)
{
	printf("BROKER EXTRA TESTS\n");
	RUN_TEST_CASE(broker_extra, should_be_safe_from_race_conditions);
}
