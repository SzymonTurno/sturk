# Sturk


[![License](https://img.shields.io/badge/License-BSD_3--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)


Sturk is a C library that implements the publish-subscribe messaging pattern. [TOC]


## Features

* User-defined message interface.
* Red-black tree for the channels dictionary.
* Memory pools for messages and queues.
* Logger with trace levels and exceptions.
* Common data structures: list, queue, dictionary.
* Single-thread implementation.
* Multi-thread implementation.


## Portability

* C99 support: `gcc -std=c99 -pedantic ...`.
* CI/CD platforms: ubuntu, macos, windows.
* Operating system abstraction layer.


## Build

Sources are compiled into a static library - `libsturk.a`.
The library uses header files from the directory `/path/to/sturk/include`.

```sh
-I/path/to/sturk/include
```


By default, `libsturk.a` will be created at `/path/to/sturk/build/src`.

```sh
cd /path/to/sturk
make
```


The build can be customized with a yaml configuration.
In this case, `libsturk.a` will be created at `/my/empty/dir`.

```sh
cd /my/empty/dir
/path/to/sturk/tools/configure.sh /path/to/config.yaml
make
```


## Message broker

Define the API for messages:

* define the size of the message,

```c
static size_t size(void)
{
	return sizeof(char[8]);
}
```


* define the message contructor,

```c
static void init(StLoad* load, va_list vlist)
{
	const char* format = va_arg(vlist, char*);

	vsnprintf(load, 8, format, vlist);
}
```


* define the message destructor,

```c
static void deinit(StLoad* load)
{
	(void)load;
}
```


* define a vtable.

```c
const struct StLoadVt SAMPLE_LOAD_API[] = {
	{.size = size, .ctor = init, .dtor = deinit}};
```


Complete API.

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


Publish and subscribe:

* initialize a broker with the chosen API,

```c
	StBroker* broker = broker_create(SAMPLE_LOAD_API);
```


* create a subscriber,

```c
	StSubscriber* sber = subscriber_create(broker);
```


* subscribe to a topic,

```c
	subscribe(sber, "test");
```


* publish to a topic,

```c
	publish(broker_search(broker, "test"), "%X", 0xF00D);
```


* receive a message.

```c
	load = subscriber_await(sber);
```


Complete example.

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
