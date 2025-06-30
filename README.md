# Cantil
Subroutines for designing portable middleware.

NOTE: this is "work in progress" library.

## 1. Build
```sh
cd /path/to/cantil && make
```

## 2. Usage

### 2.1. Design the API for your messages.
```c
static size_t size(void)
{
	return sizeof(char**);
}

static void init(CnLoad* load, va_list vlist)
{
	const char* format = va_arg(vlist, char*);
	char* buff = malloc(256);

	vsnprintf(buff, 256, format, vlist);
	*(char**)load = buff;
}

static void deinit(CnLoad* load)
{
	free(*(char**)load);
}

const struct CnLoadVt SAMPLE_LOAD_API[] = {
	{.size = size, .ctor = init, .dtor = deinit}};
```

### 2.2. Initialize publish-subscribe with your API; send and receive messages.
```c
TEST(subscriber, should_receive_enqueued_message)
{
	CnBroker* broker = broker_create(SAMPLE_LOAD_API);
	CnSubscriber* sber = subscriber_create(broker);
	CnChannel* ch = NULL;

	subscribe(sber, "test");
	publish(broker_search(broker, "test"), "%X", 0xF00D);
	TEST_ASSERT_NULL(channel_gettopic(ch));
	TEST_ASSERT_EQUAL_STRING("F00D", *(char**)subscriber_await(sber, &ch));
	TEST_ASSERT_EQUAL_STRING("test", channel_gettopic(ch));
	broker_destroy(broker);
}
```

## 3. Portability
- osal (operating system abstraction layer)
- c99 support ("`gcc -std=c99 -pedantic ...` ")

## 4. Ideas for future development
- custom heap (free list allocator)
- red-black tree deletion
- using memory as stream ("`fmemopen()`" for c99)
