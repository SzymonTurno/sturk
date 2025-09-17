# Message broker

<!--![TOC]!-->


Message broker is a data type for interprocess communication in a publish-subscribe
manner. It aggregates two other data types:

1. channels - in a dictionary (key-value data structure),
2. subscribers - in a list.

The key used in a channels dictionary is a character array (string) and it is called
a *topic* or a *named channel*. Messages are published to channels and received with
subscribers. In order for the subscriber to receive a message sent through a specific
channel it must subscribe to a topic that corresponds to this channel.

@startuml "Message broker data types"

    class Channel {
        +publish(in args : va_list)
        +gettopic() : string
    }

    class Load {
        +getchan() : Channel
    }

    abstract class LoadVt {
        +size()
        +ctor(in args : va_list) : Load
        +dtor(in load : Load)
    }

    class Broker {
        -channels : Dictionary<String, Channel>
        -subscribers : List<Subscriber>
        +create(in vp : LoadVt)
        +destroy()
        +search(in topic : String) : Channel
    }

    class Subscriber {
        +create(in broker : Broker)
        +destroy()
        +subscribe(in topic : String)
        +await() : Load
        +poll() : Load
        +unload()
    }

    Channel "many" --o "1" Broker
    Load --> Channel
    LoadVt --> Load
    Broker --> LoadVt
    Subscriber "many" --o "1" Broker
    Subscriber --> Broker
    Subscriber --> Load

@enduml


> [!note]
> Typical publish-subscribe implementations introduce also a publisher data type. This
> implementation does not have publishers. Publishing is done directly to a message
> channel.


## Define an API for messages

The size of the message, its structure and the parsing of arguments passed to the
publish procedure are controlled by the API that the broker has been created with.


### Define a size of the message

```c
static size_t size(void)
{
	return sizeof(char[8]);
}
```


### Define a message constructor<!--!{#define-a-message-constructor}!-->

```c
static void init(StLoad* load, va_list vlist)
{
	const char* format = va_arg(vlist, char*);

	vsnprintf(load, 8, format, vlist);
}
```


### Define a message destructor

```c
static void deinit(StLoad* load)
{
	(void)load;
}
```


### Define a vtable<!--!{#define-a-vtable}!-->

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


## Publish-subscribe

### Initialize a broker

```c
	StBroker* broker = broker_create(SAMPLE_LOAD_API);
```

> [!note]
> The API passed here is the previously defined [vtable](#define-a-vtable).


### Create a subscriber

```c
	StSubscriber* sber = subscriber_create(broker);
```


### Subscribe to a topic

```c
	subscribe(sber, "test");
```


### Publish to a topic

```c
	publish(broker_search(broker, "test"), "%X", 0xF00D);
```

> [!note]
> The behaviour of the publish procedure is controlled with the
> [message constructor](#define-a-message-constructor).


### Receive a message

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
