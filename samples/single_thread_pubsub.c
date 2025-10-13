#include "sample.h"
#include "st/os/sys.h"
#include "sturk/broker.h"
#include "sturk/io/logger.h"
#include "sturk/os/mem.h"
#include "sturk/str.h"

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

enum TopicId {
	INPUT_ID = 0,
	RESULT_ID,
	N_TOPICS
};

enum Event {
	STORAGE_EVENT = 0,
	MULTIPLY_EVENT,
	TRACE_EVENT,
	N_EVENTS
};

struct Payload {
	int new;
	int old;
};

struct Subscriber {
	StSubscriber* sber;
	struct StMessage msg;
};

static void receive(struct Subscriber* sub)
{
	sub->msg = subscriber_poll(sub->sber);
}

static void sample_publish(StChannel* ch, int new, int old)
{
	struct StMessage msg = channel_allocmsg(ch);
	struct Payload* pload = msg.payload;

	pload->new = new;
	pload->old = old;
	publish(&msg);
}

static void
broadcast(StChannel** ch, struct Subscriber* subs, int* store, int val)
{
	struct Payload* pload = NULL;
	int done = 0;

	trace(INFO, NULL, "broadcast %d", val);
	sample_publish(ch[INPUT_ID], val, store[INPUT_ID]);
	do {
		receive(&subs[STORAGE_EVENT]);
		receive(&subs[MULTIPLY_EVENT]);
		receive(&subs[TRACE_EVENT]);
		if (subs[STORAGE_EVENT].msg.payload) {
			pload = subs[STORAGE_EVENT].msg.payload;
			if (message_getchannel(subs[STORAGE_EVENT].msg) ==
			    ch[STORAGE_EVENT]) {
				pload->old = store[INPUT_ID];
				store[INPUT_ID] = pload->new;
			} else {
				pload->old = store[RESULT_ID];
				store[RESULT_ID] = pload->new;
			}
		}

		done = 1;
		if (subs[MULTIPLY_EVENT].msg.payload) {
			pload = subs[MULTIPLY_EVENT].msg.payload;
			sample_publish(
				ch[RESULT_ID], pload->new * pload->old,
				store[RESULT_ID]);
			done = 0;
		}

		if (subs[TRACE_EVENT].msg.payload) {
			pload = subs[TRACE_EVENT].msg.payload;
			trace(INFO, NULL, "message: new = %d, old = %d",
			      pload->new, pload->old);
		}
	} while (!done);
}

static void app(void)
{
	StBroker* broker = broker_create(sizeof(struct Payload));
	StChannel* ch[] = {
		broker_search(broker, "input"),
		broker_search(broker, "result")};
	struct Subscriber subs[] = {
		{subscriber_create(broker), {0}},
		{subscriber_create(broker), {0}},
		{subscriber_create(broker), {0}}};
	int store[ARRAY_SIZE(ch)] = {0};

	subscribe(subs[STORAGE_EVENT].sber, "input");
	subscribe(subs[STORAGE_EVENT].sber, "result");
	subscribe(subs[MULTIPLY_EVENT].sber, "input");
	subscribe(subs[TRACE_EVENT].sber, "input");
	subscribe(subs[TRACE_EVENT].sber, "result");
	broadcast(ch, subs, store, -3);
	broadcast(ch, subs, store, -13);
	broadcast(ch, subs, store, 7);
	broadcast(ch, subs, store, 1);
	broker_destroy(broker);
}

struct StStrQ* single_thread_pubsub(void)
{
	struct StStrQ* ret = NULL;
	char* out = NEW(char, 256);
	StIoBuffer* buff = NEW(StIoBuffer, iobuffer_calclen(1024));
	StIo* io = io_init(buff);

	logger_attach(INFO, io);
	app();
	logger_detach(INFO, io);
	io_putc(io, IO_EOF);
	io = io_init(buff);
	while (io_fgets(out, 256, io))
		ret = strq_ins(ret, newstr(out));
	st_free(buff);
	st_free(out);
	return ret;
}
