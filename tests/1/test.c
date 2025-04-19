#include "unity.h"
#include "middleware.h"
#include "UB/debug/log.h"
#include "UB/debug/snapshot.h"

static void broadcast(UBchan** chans, struct Subscriber* subs, int* store,
	int val)
{
	struct Payload* pl = NULL;
	int done = 0;

	log(INFO, NULL, "broadcast %d", val);
	lish(chans[0], val, 0);
	do {
		receive(&subs[0]);
		receive(&subs[1]);
		receive(&subs[2]);
		if (subs[0].pl) {
			pl = subs[0].pl;
			if (subs[0].chan == chans[0]) {
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
			lish(chans[1], pl->new * pl->old, store[1]);
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
	UBroker* broker = broker_create(PAYLOAD);
	UBchan* chans[] = {
		broker_search(broker, "input"),
		broker_search(broker, "result")
	};
	struct Subscriber subs[] = {
		{ scriber_create(broker), NULL, NULL },
		{ scriber_create(broker), NULL, NULL },
		{ scriber_create(broker), NULL, NULL }
	};
	int store[2] = {0};

	scribe(subs[0].scriber, "input");
	scribe(subs[0].scriber, "result");
	scribe(subs[1].scriber, "input");
	scribe(subs[2].scriber, "input");
	scribe(subs[2].scriber, "result");
	broadcast(chans, subs, store, -3);
	broadcast(chans, subs, store, -13);
	broadcast(chans, subs, store, 7);
	broadcast(chans, subs, store, 1);
	broker_destroy(broker);
}

extern void test_Broker_should_SupportSingleThreadDesign(void);
void test_Broker_should_SupportSingleThreadDesign(void)
{
	UBsnapshot* sshot = snapshot_prep(ACTUAL_OUT);
	UBfstream* stream = snapshot_cast(sshot);

	log_attach(INFO, stream);
	app();
	TEST_ASSERT_EQUAL(0, snapshot_ordered(sshot, EXPECTED_OUT));
	log_detach(INFO, stream);
}
