# Broker

<!--![TOC]!-->


The core of Sturk's publish-subscribe implementation is the message broker. It is
responsible for constructing messages and for allocating and deallocating all the
resources used for communication. It aggregates instances of two other data types:

1. channels - in a dictionary (key-value data structure),
2. subscribers - in a list.

The key used in a channels dictionary is a character array (string) and it is called
*topic* or *named channel*. Messages are published to channels and received with
subscribers. In order for the subscriber to receive a message sent through a channel
it must subscribe to a topic that corresponds to the channel.

The size of the message, its structure and the parsing of arguments passed to the
publish procedure are controlled by the message API, see the
example - [Define an API for messages](#define-an-api-for-messages).

| ![](http://www.plantuml.com/plantuml/proxy?cache=no&src=https://raw.githubusercontent.com/SzymonTurno/sturk/refs/heads/feat/arena-allocator/src/broker/docs/hierarchy.puml) |
| :--------------: |
| Broker hierarchy |


> [!note]
> Typical publish-subscribe implementations introduce also a publisher data type. Sturk
> implementation does not have publishers. Publishing is done directly to a message
> channel.


## Define an API for messages<!--!{#define-an-api-for-messages}!-->

In this example the message will be a sequence of pipe characters - `|`. The sequence
length will be equal to the integer passed to the publish procedure.

| ![](http://www.plantuml.com/plantuml/proxy?cache=no&src=https://raw.githubusercontent.com/SzymonTurno/sturk/refs/heads/feat/arena-allocator/src/broker/docs/message.puml) |
| :-----------------------------: |
| Message for a call "publish(5)" |


### Define the size of the message

```c
static size_t size(void)
{
	return sizeof(char*);
}
```


### Define a message constructor<!--!{#define-a-message-constructor}!-->

For `n` pipe characters allocate `n + 1` bytes (add one to include the string
terminating character - `\0`) and fill the string with `n` number of `|`.

```c
static void init(void* msg, va_list vlist)
{
	int n = va_arg(vlist, int) + 1;
	char* str = NULL;

	if (n > 0) {
		str = malloc((size_t)n);
		str[--n] = '\0';
		while (n--)
			str[n] = '|';
	}
	*(char**)msg = str;
}
```


### Define a message destructor

```c
static void deinit(void* msg)
{
	free(*(char**)msg);
}
```


### Define a vtable<!--!{#define-a-vtable}!-->

```c
const struct StMessageVt SAMPLE_MESSAGE_API[] = {
	{.size = size, .ctor = init, .dtor = deinit}};
```


### Complete API

```c
static size_t size(void)
{
	return sizeof(char*);
}

static void init(void* msg, va_list vlist)
{
	int n = va_arg(vlist, int) + 1;
	char* str = NULL;

	if (n > 0) {
		str = malloc((size_t)n);
		str[--n] = '\0';
		while (n--)
			str[n] = '|';
	}
	*(char**)msg = str;
}

static void deinit(void* msg)
{
	free(*(char**)msg);
}

const struct StMessageVt SAMPLE_MESSAGE_API[] = {
	{.size = size, .ctor = init, .dtor = deinit}};
```


## Publish-subscribe

### Initialize a broker

```c
	StBroker* broker = broker_create(SAMPLE_MESSAGE_API);
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
	publish(broker_search(broker, "test"), 5);
```

> [!note]
> The behaviour of the publish procedure is controlled with the
> [message constructor](#define-a-message-constructor).


### Receive a message

```c
	msg = subscriber_await(sber);
```


### Complete example

```c
TEST(subscriber, should_receive_enqueued_message)
{
	StBroker* broker = broker_create(SAMPLE_MESSAGE_API);
	StSubscriber* sber = subscriber_create(broker);
	char** msg = NULL;

	subscribe(sber, "test");
	publish(broker_search(broker, "test"), 5);
	msg = subscriber_await(sber);
	TEST_ASSERT_EQUAL_STRING("|||||", *msg);
	broker_destroy(broker);
}
```
