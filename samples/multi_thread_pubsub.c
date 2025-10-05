#include "pubsub.h"
#include "st/os/sys.h"
#include "sturk/broker.h"
#include "sturk/logger/trace.h"
#include "sturk/os/mem.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define MULT_TH_ID  0
#define REP_TH_ID   1
#define DELIM_TH_ID 2

struct Payload {
	int new;
	int old;
	StChannel* channel;
};

struct Publisher {
	StBroker* broker;
	union {
		int data;
		void* align;
	} u;
};

struct Subscriber {
	StSubscriber* sber;
	struct Payload* pl;
};

static size_t getsize(void)
{
	return sizeof(struct Payload);
}

static void init(void* msg, va_list va)
{
	((struct Payload*)msg)->new = va_arg(va, int);
	((struct Payload*)msg)->old = va_arg(va, int);
	((struct Payload*)msg)->channel = va_arg(va, StChannel*);
}

static void deinit(void* msg)
{
	(void)msg;
}

const struct StMessageVt PAYLOAD_API[] = {
	{.size_cb = getsize, .ctor = init, .dtor = deinit}};

static int receive(struct Subscriber* sub)
{
	sub->pl = subscriber_await(sub->sber);
	return 1;
}

static void sample_publish(struct Publisher* pub, const char* topic, int data)
{
	StChannel* ch = broker_search(pub->broker, topic);

	publish(ch, data, pub->u.data, ch);
	pub->u.data = data;
}

static int join_requested(struct Subscriber* sub, int i)
{
	if (strcmp(channel_gettopic(sub->pl->channel), "join"))
		return 0;
	return sub->pl->new == i;
}

static void* multiply(void* arg)
{
	StBroker* broker = arg;
	struct Publisher pub = {.broker = broker, .u.data = 0};
	struct Subscriber sub = {subscriber_create(broker), NULL};

	subscribe(sub.sber, "input");
	subscribe(sub.sber, "join");
	sample_publish(&pub, "ready", MULT_TH_ID);
	while (receive(&sub)) {
		if (join_requested(&sub, MULT_TH_ID)) {
			subscriber_destroy(sub.sber);
			break;
		}
		/* clang-format off */
		sample_publish(&pub, "result", sub.pl->new * sub.pl->old);
		/* clang-format on */
	}
	sample_publish(&pub, "done", MULT_TH_ID);
	return NULL;
}

static void* report(void* arg)
{
	StBroker* broker = arg;
	struct Publisher pub = {.broker = broker, .u.data = 0};
	struct Subscriber sub = {subscriber_create(broker), NULL};

	subscribe(sub.sber, "input");
	subscribe(sub.sber, "result");
	subscribe(sub.sber, "join");
	sample_publish(&pub, "ready", REP_TH_ID);
	while (receive(&sub)) {
		if (join_requested(&sub, REP_TH_ID)) {
			subscriber_destroy(sub.sber);
			break;
		}
		trace(INFO, NULL, "message: new = %d, old = %d", sub.pl->new,
		      sub.pl->old);
	}
	sample_publish(&pub, "done", REP_TH_ID);
	return NULL;
}

static void
start_thread(StBroker* broker, pthread_t* thid, int i, void* (*cb)(void*))
{
	struct Subscriber sub = {subscriber_create(broker), NULL};
	pthread_attr_t attr;

	subscribe(sub.sber, "ready");
	pthread_attr_init(&attr);
	pthread_attr_setstacksize(&attr, 512);
	pthread_create(&thid[i], &attr, cb, broker);
	pthread_attr_destroy(&attr);
	while (receive(&sub) && sub.pl->new != i)
		;
}

static void join_thread(StBroker* broker, pthread_t* thid, int i)
{
	struct Publisher pub = {.broker = broker, .u.data = 0};
	struct Subscriber sub = {subscriber_create(broker), NULL};
	void* res = NULL;

	subscribe(sub.sber, "done");
	sample_publish(&pub, "join", i);
	while (receive(&sub) && sub.pl->new != i)
		;
	pthread_join(thid[i], &res);
	free(res);
}

static void app(void)
{
	StBroker* broker = broker_create(PAYLOAD_API);
	struct Publisher pub = {.broker = broker, .u.data = 0};
	pthread_t thid[DELIM_TH_ID] = {0};

	start_thread(broker, thid, REP_TH_ID, report);
	start_thread(broker, thid, MULT_TH_ID, multiply);
	sample_publish(&pub, "input", -3);
	sample_publish(&pub, "input", -13);
	sample_publish(&pub, "input", 7);
	sample_publish(&pub, "input", 1);
	join_thread(broker, thid, MULT_TH_ID);
	join_thread(broker, thid, REP_TH_ID);
	broker_destroy(broker);
}

struct StStrBag* multi_thread_pubsub(void)
{
	struct StStrBag* ret = NULL;
	char* out = NEW(char, 256);
	StIoBuffer* buff = NEW(StIoBuffer, iobuffer_getlen(1024));
	StIo* io = io_init(buff);

	logger_attach(INFO, io);
	app();
	logger_detach(INFO, io);
	io_putc(io, IO_EOF);
	io = io_init(buff);
	while (io_fgets(out, 256, io))
		ret = st_strbag_ins(ret, out);
	st_free(buff);
	st_free(out);
	return ret;
}
