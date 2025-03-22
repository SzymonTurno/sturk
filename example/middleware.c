#include "middleware.h"

static size_t size(void)
{
	return sizeof(struct Payload);
}

static UBload* init(UBload* load, va_list vlist)
{
	((struct Payload*)load)->new = va_arg(vlist, int);
	((struct Payload*)load)->old = va_arg(vlist, int);
	return load;
}

const struct UBloadVt PAYLOAD[] = {{
	.size = size,
	.ctor = init,
}};

void receive(struct Subscriber* sub)
{
	sub->pl = (struct Payload*)ub_scriber_await(sub->scriber, &sub->chan);
}
