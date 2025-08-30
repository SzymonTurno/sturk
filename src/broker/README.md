# Message broker

Below is an example usage of the message broker. [TOC]


## Defining API for messages


### Defining the size of the message

```c
static size_t size(void)
{
	return sizeof(char[8]);
}
```


### Defining the message constructor {#defining-the-message-constructor}

```c
static void init(StLoad* load, va_list vlist)
{
	const char* format = va_arg(vlist, char*);

	vsnprintf(load, 8, format, vlist);
}
```


### Defining the message destructor

```c
static void deinit(StLoad* load)
{
	(void)load;
}
```


### Defining a vtable {#defining-a-vtable}

```c
const struct StLoadVt SAMPLE_LOAD_API[] = {
	{.size = size, .ctor = init, .dtor = deinit}};
```


### Complete API

```c
static size_t size(void)
{
	return sizeof(char[8]);
}

static void init(StLoad* load, va_list vlist)
{
	const char* format = va_arg(vlist, char*);

	vsnprintf(load, 8, format, vlist);
}

static void deinit(StLoad* load)
{
	(void)load;
}

const struct StLoadVt SAMPLE_LOAD_API[] = {
	{.size = size, .ctor = init, .dtor = deinit}};
```


## Publishing and subscribing

### Initializing a broker

```c
	StBroker* broker = broker_create(SAMPLE_LOAD_API);
```

@note
> [!NOTE]
> The API passed here is the previously defined [vtable](#defining-a-vtable).


### Creating a subscriber

```c
	StSubscriber* sber = subscriber_create(broker);
```


### Subscribing to a topic

```c
	subscribe(sber, "test");
```


### Publishing to a topic

```c
	publish(broker_search(broker, "test"), "%X", 0xF00D);
```

@note
> [!NOTE]
> The behaviour of the publish procedure is controlled with the
> [message constructor](#defining-the-message-constructor).


### Receiving a message

```c
	load = subscriber_await(sber);
```


### Complete example

```c
TEST(subscriber, should_receive_enqueued_message)
{
	StBroker* broker = broker_create(SAMPLE_LOAD_API);
	StSubscriber* sber = subscriber_create(broker);
	StLoad* load = NULL;

	subscribe(sber, "test");
	publish(broker_search(broker, "test"), "%X", 0xF00D);
	TEST_ASSERT_NULL(channel_gettopic(load_getchan(load)));
	load = subscriber_await(sber);
	TEST_ASSERT_EQUAL_STRING("F00D", load);
	TEST_ASSERT_NULL("test", channel_gettopic(load_getchan(load)));
	broker_destroy(broker);
}
```
