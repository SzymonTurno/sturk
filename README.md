# Cantil
Subroutines for designing portable middleware solutions.
Based on the publish-subscribe pattern.

## Build
```sh
cd /path/to/cantil && make
```

## Usage
1. Design the API for your messages.
```c
static size_t size(void)
{
	return sizeof(char**);
}

static void init(CnLoad* load, va_list vlist)
{
	char* buff = malloc(256);

	vsnprintf(buff, 256, va_arg(vlist, char*), vlist);
	*(char**)load = buff;
}

static void deinit(CnLoad* load)
{
	free(*(char**)load);
}

const struct CnLoadVt SAMPLE_LOAD_API[] = {
	{.size = size, .ctor = init, .dtor = deinit}};
```
2. Initialize publish-subscribe with your API; send and receive messages.
```c
TEST(subscriber, should_receive_enqueued_message)
{
	CnBroker* broker = broker_create(SAMPLE_LOAD_API);
	CnSubscriber* sber = subscriber_create(broker);
	CnChannel* ch = NULL;

	subscribe(sber, "test");
	publish(broker_search(broker, "test"), "%d", 3212);
	TEST_ASSERT_NULL(get_topic(ch));
	TEST_ASSERT_EQUAL_STRING("3212", *(char**)subscriber_await(sber, &ch));
	TEST_ASSERT_EQUAL_STRING("test", get_topic(ch));
	broker_destroy(broker);
}
```

## Portability
- osal (operating system abstraction layer)
- c99 support

## Ideas for future development
- custom heap (free list allocator)
- red-black tree deletion
- open memory as stream for c99 (fmemopen())
