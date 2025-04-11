#include "ub/logger.h"
#include "middleware.h"
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

#define MULT_TH_ID  0
#define REP_TH_ID   1
#define DELIM_TH_ID 2

static int join_requested(struct Subscriber* sub, int i)
{
	if (strcmp(ub_get_topic(sub->chan), "join"))
		return 0;
	return sub->pl->new == i;
}

static void* multiply(void* arg)
{
	UBroker* broker = arg;
	struct Publisher pub = {.broker = broker, .u.data = 0};
	struct Subscriber sub = {ub_scriber_create(broker), NULL, NULL};

	ub_scribe(sub.scriber, "input");
	ub_scribe(sub.scriber, "join");
	publish(&pub, "ready", MULT_TH_ID);
	while (receive(&sub)) {
		if (join_requested(&sub, MULT_TH_ID)) {
			ub_scriber_destroy(sub.scriber);
			break;
		}
		publish(&pub, "result", sub.pl->new * sub.pl->old);
	}
	publish(&pub, "done", MULT_TH_ID);
	return NULL;
}

static void* report(void* arg)
{
	UBroker* broker = arg;
	struct Publisher pub = {.broker = broker, .u.data = 0};
	struct Subscriber sub = {ub_scriber_create(broker), NULL, NULL};

	ub_scribe(sub.scriber, "input");
	ub_scribe(sub.scriber, "result");
	ub_scribe(sub.scriber, "join");
	publish(&pub, "ready", REP_TH_ID);
	while (receive(&sub)) {
		if (join_requested(&sub, REP_TH_ID)) {
			ub_scriber_destroy(sub.scriber);
			break;
		}
		ub_log(UB_INFO, NULL, "message: new = %d, old = %d",
		       sub.pl->new, sub.pl->old);
	}
	publish(&pub, "done", REP_TH_ID);
	return NULL;
}

static void
start_thread(UBroker* broker, pthread_t* thid, int i, void* (*cb)(void*))
{
	struct Subscriber sub = {ub_scriber_create(broker), NULL, NULL};
	pthread_attr_t attr;

	ub_scribe(sub.scriber, "ready");
	pthread_attr_init(&attr);
	pthread_attr_setstacksize(&attr, 512);
	pthread_create(&thid[i], &attr, cb, broker);
	pthread_attr_destroy(&attr);
	while (receive(&sub) && sub.pl->new != i)
		;
}

static void join_thread(UBroker* broker, pthread_t* thid, int i)
{
	struct Publisher pub = {.broker = broker, .u.data = 0};
	struct Subscriber sub = {ub_scriber_create(broker), NULL, NULL};
	void* res = NULL;

	ub_scribe(sub.scriber, "done");
	publish(&pub, "join", i);
	while (receive(&sub) && sub.pl->new != i)
		;
	pthread_join(thid[i], &res);
	free(res);
}

int main(void)
{
	UBroker* broker = ub_broker_create(PAYLOAD);
	struct Publisher pub = {.broker = broker, .u.data = 0};
	pthread_t thid[DELIM_TH_ID] = {0};

	start_thread(broker, thid, REP_TH_ID, report);
	start_thread(broker, thid, MULT_TH_ID, multiply);
	publish(&pub, "input", -3);
	publish(&pub, "input", -13);
	publish(&pub, "input", 7);
	publish(&pub, "input", 1);
	join_thread(broker, thid, MULT_TH_ID);
	join_thread(broker, thid, REP_TH_ID);
	ub_broker_destroy(broker);
	return 0;
}
