#include "cantil/broker.h"
#include "cantil/logger/trace.h"
#include "cantil/str.h"
#include "cantil/os/mem.h"
#include "cn/os/sys.h"
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
	CnSubscriber* sber;
	struct Payload* pload;
	CnChannel* channel;
};

static size_t size(void)
{
	return sizeof(struct Payload);
}

static void init(CnLoad* load, va_list vlist)
{
	((struct Payload*)load)->new = va_arg(vlist, int);
	((struct Payload*)load)->old = va_arg(vlist, int);
}

static void deinit(CnLoad* load)
{
	(void)load;
}

static const struct CnLoadVt PAYLOAD_API[] = {
	{.size = size, .ctor = init, .dtor = deinit}};

static void receive(struct Subscriber* sub)
{
	sub->pload = (struct Payload*)subscriber_poll(sub->sber, &sub->channel);
}

static void
broadcast(CnChannel** ch, struct Subscriber* subs, int* store, int val)
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
	CnBroker* broker = broker_create(PAYLOAD_API);
	CnChannel* ch[] = {
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

struct CnStrq* single_thread_pubsub(void)
{
	struct CnStrq* ret = NULL;
	struct CnFstream* stream = cn_fopen("single_thread_pubsub.tmp", "w+");
	char* buff = NEW(char, 256);

	logger_attach(INFO, stream);
	app();
	logger_detach(INFO, stream);
	cn_fseekset(stream, 0);
	while (cn_fgets(buff, 256, stream))
		ret = strq_ins(ret, newstr(buff));
	cn_free(buff);
	cn_fclose(stream);
	cn_remove("single_thread_pubsub.tmp");
	return ret;
}
