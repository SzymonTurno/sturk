#include "sturk/broker.h"
#include "sturk/logger/trace.h"
#include "sturk/os/mem.h"
#include "sturk/str.h"
#include "st/os/sys.h"
#include "pubsub.h"

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
	struct Payload* pload;
	StChannel* channel;
};

static size_t size(void)
{
	return sizeof(struct Payload);
}

static void init(StLoad* load, va_list vlist)
{
	((struct Payload*)load)->new = va_arg(vlist, int);
	((struct Payload*)load)->old = va_arg(vlist, int);
}

static void deinit(StLoad* load)
{
	(void)load;
}

static const struct StLoadVt PAYLOAD_API[] = {
	{.size = size, .ctor = init, .dtor = deinit}};

static void receive(struct Subscriber* sub)
{
	StLoad* load = subscriber_poll(sub->sber);

	sub->pload = (struct Payload*)load;
	sub->channel = load_getchan(load);
}

static void
broadcast(StChannel** ch, struct Subscriber* subs, int* store, int val)
{
	struct Payload* pload = NULL;
	int done = 0;

	trace(INFO, NULL, "broadcast %d", val);
	publish(ch[INPUT_ID], val, store[INPUT_ID]);
	do {
		receive(&subs[STORAGE_EVENT]);
		receive(&subs[MULTIPLY_EVENT]);
		receive(&subs[TRACE_EVENT]);
		if (subs[STORAGE_EVENT].pload) {
			pload = subs[STORAGE_EVENT].pload;
			if (subs[STORAGE_EVENT].channel == ch[STORAGE_EVENT]) {
				pload->old = store[INPUT_ID];
				store[INPUT_ID] = pload->new;
			} else {
				pload->old = store[RESULT_ID];
				store[RESULT_ID] = pload->new;
			}
		}

		done = 1;
		if (subs[MULTIPLY_EVENT].pload) {
			pload = subs[MULTIPLY_EVENT].pload;
			publish(ch[RESULT_ID], pload->new * pload->old,
			        store[RESULT_ID]);
			done = 0;
		}

		if (subs[TRACE_EVENT].pload) {
			pload = subs[TRACE_EVENT].pload;
			trace(INFO, NULL, "message: new = %d, old = %d",
			      pload->new, pload->old);
		}
	} while (!done);
}

static void app(void)
{
	StBroker* broker = broker_create(PAYLOAD_API);
	StChannel* ch[] = {
		broker_search(broker, "input"),
		broker_search(broker, "result")};
	struct Subscriber subs[] = {
		{subscriber_create(broker), NULL, NULL},
		{subscriber_create(broker), NULL, NULL},
		{subscriber_create(broker), NULL, NULL}};
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

struct StStrq* single_thread_pubsub(void)
{
	struct StStrq* ret = NULL;
	struct StFstream* stream = st_fopen("single_thread_pubsub.tmp", "w+");
	char* buff = NEW(char, 256);

	logger_attach(INFO, stream);
	app();
	logger_detach(INFO, stream);
	st_fseekset(stream, 0);
	while (st_fgets(buff, 256, stream))
		ret = strq_ins(ret, newstr(buff));
	st_free(buff);
	st_fclose(stream);
	st_remove("single_thread_pubsub.tmp");
	return ret;
}
