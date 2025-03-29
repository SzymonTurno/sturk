#include "middleware.h"

static size_t size(void)
{
	return sizeof(struct Payload);
}

static void init(UBload* load, va_list vlist)
{
	((struct Payload*)load)->new = va_arg(vlist, int);
	((struct Payload*)load)->old = va_arg(vlist, int);
}

static void deinit(UBload* load)
{
	(void)load;
}

const struct UBloadVt PAYLOAD[] = {{
	.size = size,
	.ctor = init,
	.dtor = deinit
}};

void receive(struct Subscriber* sub)
{
	sub->pl = (struct Payload*)ub_scriber_await(sub->scriber, &sub->chan);
}
