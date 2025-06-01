#include "pubsub.h"
#include "cn/os/mem.h"
#include "cn/os/sys.h"
#include "cantil/logger/log.h"
#include "cantil/broker.h"
#include "cantil/str.h"

struct Payload {
	int new;
	int old;
};

struct Subscriber {
	CnSubscriber* sber;
	struct Payload* pl;
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

static const struct CnLoadVt PAYLOAD[] = {{
	.size = size,
	.ctor = init,
	.dtor = deinit
}};

static void receive(struct Subscriber* sub)
{
	sub->pl = (struct Payload*)subscriber_poll(sub->sber, &sub->channel);
}

static void broadcast(CnChannel** ch, struct Subscriber* subs, int* store,
	int val)
{
	struct Payload* pl = NULL;
	int done = 0;

	log(INFO, NULL, "broadcast %d", val);
	publish(ch[0], val, store[0]);
	do {
		receive(&subs[0]);
		receive(&subs[1]);
		receive(&subs[2]);
		if (subs[0].pl) {
			pl = subs[0].pl;
			if (subs[0].channel == ch[0]) {
				pl->old = store[0];
				store[0] = pl->new;
			} else {
				pl->old = store[1];
				store[1] = pl->new;
			}
		}

		done = 1;
		if (subs[1].pl) {
			pl = subs[1].pl;
			publish(ch[1], pl->new * pl->old, store[1]);
			done = 0;
		}

		if (subs[2].pl) {
			pl = subs[2].pl;
			log(INFO, NULL, "message: new = %d, old = %d", pl->new,
				pl->old);
		}
	} while (!done);
}

static void app(void)
{
	CnBroker* broker = broker_create(PAYLOAD);
	CnChannel* ch[] = {
		broker_search(broker, "input"),
		broker_search(broker, "result")
	};
	struct Subscriber subs[] = {
		{ subscriber_create(broker), NULL, NULL },
		{ subscriber_create(broker), NULL, NULL },
		{ subscriber_create(broker), NULL, NULL }
	};
	int store[2] = {0};

	subscribe(subs[0].sber, "input");
	subscribe(subs[0].sber, "result");
	subscribe(subs[1].sber, "input");
	subscribe(subs[2].sber, "input");
	subscribe(subs[2].sber, "result");
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
	char* buff = cn_malloc(256);

	log_attach(INFO, stream);
	app();
	log_detach(INFO, stream);
	cn_fseekset(stream, 0);
	while (cn_fgets(buff, 256, stream))
		ret = strq_ins(ret, newstr(buff));
	cn_free(buff);
	cn_fclose(stream);
	cn_remove("single_thread_pubsub.tmp");
	return ret;
}
