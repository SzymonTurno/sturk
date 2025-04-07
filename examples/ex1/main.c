#include "middleware.h"
#include "ub/logger.h"

static void broadcast(UBchan** chans, struct Subscriber* subs, int* store,
	int val)
{
	struct Payload* pl = NULL;
	int done = 0;

	ub_log(UB_INFO, NULL, "broadcast %d", val);
	ub_lish(chans[0], val, 0);
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
			ub_lish(chans[1], pl->new * pl->old, store[1]);
			done = 0;
		}

		if (subs[2].pl) {
			pl = subs[2].pl;
			ub_log(UB_INFO, NULL, "message: new = %d, old = %d",
				pl->new, pl->old);
		}
	} while (!done);
}

int main(void)
{
	UBroker* broker = ub_broker_create(PAYLOAD);
	UBchan* chans[] = {
		ub_broker_search(broker, "input"),
		ub_broker_search(broker, "result")
	};
	struct Subscriber subs[] = {
		{ub_scriber_create(broker), NULL, NULL},
		{ub_scriber_create(broker), NULL, NULL},
		{ub_scriber_create(broker), NULL, NULL}
	};
	int store[2] = {0};

	ub_scribe(subs[0].scriber, "input");
	ub_scribe(subs[0].scriber, "result");
	ub_scribe(subs[1].scriber, "input");
	ub_scribe(subs[2].scriber, "input");
	ub_scribe(subs[2].scriber, "result");
	ub_log_open(EXPECTED_OUT);
	broadcast(chans, subs, store, -3);
	broadcast(chans, subs, store, -13);
	broadcast(chans, subs, store, 7);
	broadcast(chans, subs, store, 1);
	ub_log_close();
	ub_broker_destroy(broker);
	return 0;
}
