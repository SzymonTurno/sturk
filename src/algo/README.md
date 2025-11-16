# Algo

## Memory pool

```c
/**
 * @var typedef struct StPool StPool
 *
 * @brief Memory pool.
 */
typedef struct StPool StPool;
```

## Waiting queue

```c
/**
 * @var typedef struct StWaitQ StWaitQ
 *
 * @brief Waiting queue.
 */
typedef struct StWaitQ StWaitQ;
```

## Dictionary

### Define a dictionary data type

```c
typedef SomeData MyData;

ST_DICT(struct MyDict, MyData);
```

### Dictionary example "push"

```c
void push(struct MyDict** dictp, const char* key, MyData val)
{
        struct MyDict* entry = malloc(sizeof(*entry));

        st_dict_setk(entry, key);
        *st_dict_datap(entry) = val;
        *dictp = st_dict_ins(*dictp, entry);
}
```

## List of strings

```c
/**
 * @def VX_LIST(struct StStrList, char*)
 *
 * @struct StStrList
 *
 * @brief List of strings.
 */
VX_LIST(struct StStrList, char*);
/**< This is a macro definition of the list of strings data type. */
```

### List of strings example

```c
TEST(algo, should_implement_list_as_lifo)
{
        struct StStrList* list = NULL;

        list = strlist_ins(NULL, "One");
        list = strlist_ins(list, "Two");
        list = strlist_ins(list, "Three");
        TEST_ASSERT_EQUAL_STRING("Three", strlist_rem(&list));
        TEST_ASSERT_EQUAL_STRING("Two", strlist_rem(&list));
        TEST_ASSERT_EQUAL_STRING("One", strlist_rem(&list));
        TEST_ASSERT_NULL(list);
}
```

## Queue of strings

```c
/**
 * @def VX_CIRQ(struct StStrQ, char*)
 *
 * @struct StStrQ
 *
 * @brief Queue of strings.
 */
VX_CIRQ(struct StStrQ, char*);
/**< This is a macro definition of the queue of strings data type. */
```

### Queue of strings example

```c
TEST(algo, should_implement_cirq_as_fifo)
{
        struct StStrQ* q = NULL;

        q = strq_ins(NULL, "One");
        q = strq_ins(q, "Two");
        q = strq_ins(q, "Three");
        TEST_ASSERT_EQUAL_STRING("One", strq_rem(&q));
        TEST_ASSERT_EQUAL_STRING("Two", strq_rem(&q));
        TEST_ASSERT_EQUAL_STRING("Three", strq_rem(&q));
        TEST_ASSERT_NULL(q);
}
```

## Bag of strings

```c
/**
 * @def ST_DICT(struct StStrBag, union {int n; void* align;})
 *
 * @struct StStrBag
 *
 * @brief Bag of strings.
 */
ST_DICT(struct StStrBag, union {int n; void* align;});
/**< This is a macro definition of the bag of strings data type. */
```

### Bag of strings example

```c
TEST(algo, should_count_with_strbag)
{
        struct StStrBag* bag = NULL;

        bag = strbag_ins(NULL, "Alice");
        bag = strbag_ins(bag, "Bob");
        bag = strbag_ins(bag, "Bob");
        bag = strbag_rem(bag, "Ted");
        TEST_ASSERT_EQUAL_INT(1, strbag_count(dict_find(bag, "Alice")));
        TEST_ASSERT_EQUAL_INT(2, strbag_count(dict_find(bag, "Bob")));
        TEST_ASSERT_EQUAL_INT(0, strbag_count(dict_find(bag, "Carol")));
        TEST_ASSERT_EQUAL_INT(-1, strbag_count(dict_find(bag, "Ted")));
        strbag_destroy(bag);
}
```

> [!note]
> The bag of strings data type inherits from the dictionary data type and thus,
> can be used with `dict_find()`.
