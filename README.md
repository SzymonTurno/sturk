# Sturk

[![License](https://img.shields.io/badge/License-BSD_3--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)

[Code](https://github.com/SzymonTurno/sturk)/[Documentation](https://szymonturno.github.io/sturk)

Scalable publish-subscribe implementation.

***Note, this project is under active development.***

## 1. Features
- **User-defined** message interface.
- **Red-black tree** for the channels dictionary.
- **Memory pools** for messages and queues.
- **Logger** with trace levels and exceptions.
- **Common data structures**: list, queue, dictionary.
- **Single-thread** implementation.
- **Multi-thread** implementation.

## 2. Portability
- **Platforms**: ubuntu, macos, windows.
- **OSAL**: operating system abstraction layer.
- **C99 support**: `gcc -std=c99 -pedantic ...`.

## 3. Build
| Output file |
| ----------- |
| libsturk.a  |
```sh
# default build
cd /path/to/sturk
make

# customized build
cd /my/empty/dir
/path/to/sturk/tools/configure.sh /path/to/config.yaml
make
```

## 4. Message broker

### 4.1. Define the message interface
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

### 4.2. Initialize the message broker
```
	StBroker* broker = broker_create(SAMPLE_LOAD_API);
```

### 4.3. Exchange messages
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
