# Broker

<!--![TOC]!-->

The core of Sturk's publish-subscribe implementation is the message broker which
is responsible for constructing and routing messages. It aggregates instances of
two data types:

1. channels - in a dictionary (key-value data structure),
2. subscribers.

The key used in a channels dictionary is a character array (string) and it is called
*topic* or *named channel*. Messages are published to channels and received with
subscribers. In order for the subscriber to receive a message sent through a channel
it must subscribe to a topic that corresponds to the channel.

| ![broker-hierarchy](http://www.plantuml.com/plantuml/proxy?cache=no&src=https://raw.githubusercontent.com/SzymonTurno/sturk/refs/heads/main/src/broker/docs/hierarchy.puml) |
| :--------------: |
| Broker hierarchy |

## Broker example

In this example the payload is a single string.

### Create a broker

Use `sizeof(char*)` as a size of the payload.

```c
        StBroker* broker = broker_create(sizeof(char*));
```

### Create a subscriber

```c
        StSubscriber* sber = subscriber_create(broker);
```

### Create a channel

```c
        StChannel* ch = broker_search(broker, "test.topic");
```

### Subscribe to a topic

```c
        subscribe(sber, "test.topic");
```

### Expand the message pool

Put one message in the pool.

```c
        broker_adjust(broker, 1);
```

> [!attention]
> The message pool of a freshly created broker is empty. Make sure that you placed
> some messages in the pool - empty pool means that the communication is stuck.

### Allocate a message from the message pool

```c
        msg = message_alloc(ch);
```

### Initialize the payload<!--!{#initialize-the-payload}!-->

```c
        *(char**)msg.payload = st_strdup("Alice");
```

### Set the message callback

Set the callback function to perform some cleanup when the message is
returned to the pool.

```c
        message_setcb(msg, test_freepayload);
```

> [!note]
> The memory allocated at [Initialize the payload](#initialize-the-payload)
> is freed by the message callback.

```c
static void test_freepayload(struct StMessage msg)
{
        st_free(*(char**)msg.payload);
}
```

### Publish a message

```c
        publish(&msg);
```

### Receive a message

```c
        msg = subscriber_poll(sber);
```

### Cleanup

```c
        broker_destroy(broker);
```

### Complete example

```c
static void test_freepayload(struct StMessage msg)
{
        st_free(*(char**)msg.payload);
}

TEST(broker, should_forward_message)
{
        StBroker* broker = broker_create(sizeof(char*));
        StSubscriber* sber = subscriber_create(broker);
        StChannel* ch = broker_search(broker, "test.topic");
        struct StMessage msg = {0};

        /* Subscribe */
        subscribe(sber, "test.topic");
        TEST_ASSERT_NULL(subscriber_poll(sber).payload);

        /* Publish */
        broker_adjust(broker, 1);
        msg = message_alloc(ch);
        *(char**)msg.payload = st_strdup("test contents");
        message_setcb(msg, test_freepayload);
        publish(&msg);
        TEST_ASSERT_NULL(msg.payload);

        /* Receive */
        msg = subscriber_poll(sber);
        TEST_ASSERT_EQUAL_STRING("test contents", *(char**)msg.payload);

        /* Cleanup */
        broker_destroy(broker);
}
```
