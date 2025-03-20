#include "middleware.h"
#include "ub/mom/lish.h"
#include "ub/logger.h"
#include <stdio.h>

static void persist(void* instance, struct Payload* pl)
{
	int* val = instance;

	pl->old = *val;
	*val = pl->new;
}

static void multiply(void* instance, struct Payload* pl)
{
	ub_lish(ub_mess_create(instance, pl->new * pl->old));
}

static void report(void* instance, struct Payload* pl)
{
	(void)instance;
	ub_log(UB_INFO, NULL, "message: new = %d, old = %d", pl->new, pl->old);
}

int main(void)
{
	UBloader* loader = ub_loader_create(PAYLOAD);
	UBchan* input = ub_chan_create(loader);
	UBchan* result = ub_chan_create(loader);
	int input_val = 0;
	int result_val = 0;
	struct UBscriber* sub1 = create_subscriber(&input_val, persist);
	struct UBscriber* sub2 = create_subscriber(&result_val, persist);
	struct UBscriber* sub3 = create_subscriber(result, multiply);
	struct UBscriber* sub4 = create_subscriber(NULL, report);

	ub_scribe(sub1, input, 0);
	ub_scribe(sub2, result, 0);
	ub_scribe(sub3, input, 2);
	ub_scribe(sub4, input, 1);
	ub_scribe(sub4, result, 1);

	ub_log_open("example/expected.out");

	ub_log(UB_INFO, NULL, "broadcast -3");
	ub_lish(ub_mess_create(input, -3));
	ub_log(UB_INFO, NULL, "broadcast -13");
	ub_lish(ub_mess_create(input, -13));
	ub_log(UB_INFO, NULL, "broadcast 7");
	ub_lish(ub_mess_create(input, 7));
	ub_log(UB_INFO, NULL, "broadcast 1");
	ub_lish(ub_mess_create(input, 1));

	ub_log_close();

	ub_scriber_destroy(sub4);
	ub_scriber_destroy(sub3);
	ub_scriber_destroy(sub2);
	ub_scriber_destroy(sub1);
	ub_chan_destroy(result);
	ub_chan_destroy(input);
	ub_loader_destroy(loader);

	return 0;
}
