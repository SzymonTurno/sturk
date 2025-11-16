# Basis

## Arena

```c
/**
 * @var typedef struct StArena StArena
 *
 * @brief The arena.
 */
typedef struct StArena StArena;
```

## IO interface

```c
/**
 * @var typedef union StIo StIo
 *
 * @brief The IO interface.
 */
typedef union StIo StIo;
```

### Memory buffer example

```c
TEST(basis, should_write_to_memory_buffer)
{
        char out[16] = {0};

        /* Allocate sixteen bytes plus metadata for the memory buffer. */
        StIoBuffer buff[iobuffer_calclen(sizeof(out))] = {0};

        /* Initialize the memory buffer. */
        StIo* io = io_init(buff);

        /* Print "deadbeef" to the memory buffer. */
        io_print(io, "%x", 0xDEADBEEF);

        /* Put EOF character in the memory buffer. */
        io_putc(io, IO_EOF);

        /* Initialize the memory buffer again to read from the beginning. */
        io = io_init(buff);

        /* Read the buffer. */
        TEST_ASSERT_EQUAL_STRING("deadbeef", io_fgets(out, sizeof(out), io));
}
```

### IO operations

```c
/**
 * @struct StIoVt
 *
 * @brief Vtable for IO operations.
 */
struct StIoVt {
        /**
         * @var void (*putc_cb)(void*, char)
         *
         * @brief Callback for writing a single char.
         */
        void (*putc_cb)(void*, char);

        /**
         * @var char (*getc_cb)(void*)
         *
         * @brief Callback for reading a single char.
         */
        char (*getc_cb)(void*);
};
```

### Reading from a file

#### Define the IO operations Vtable

```c
static void file_putc(void* p, char c)
{
        FILE* fp = p;

        putc(c, fp);
}

static char file_getc(void* p)
{
        FILE* fp = p;

        return (char)getc(fp);
}

const struct StIoVt SAMPLE_FILE_API[] = {
        {.putc_cb = file_putc, .getc_cb = file_getc}};
```

#### Configure the IO interface

```c
        StIoBuffer buff[iobuffer_calclen(0)] = {0};
        StIo* io = io_init(buff);
        FILE* fp = fopen(LOREM_TXT, "r");

        io_setp(io, fp);
        io_setvp(io, SAMPLE_FILE_API);
```

#### Reading from a file example

```c
TEST(basis, should_read_from_file)
{
        const char* expected[] = {
                "Lorem ipsum dolor sit amet, consectetur adipiscing\n",
                "elit, sed do eiusmod tempor incididunt ut labore\n",
                "et dolore magna aliqua. Ut enim ad minim veniam,\n",
                "quis nostrud exercitation ullamco laboris nisi ut\n",
                "aliquip ex ea commodo consequat.\n"};
        char out[256] = {0};
        StIoBuffer buff[iobuffer_calclen(0)] = {0};
        StIo* io = io_init(buff);
        FILE* fp = fopen(LOREM_TXT, "r");

        io_setp(io, fp);
        io_setvp(io, SAMPLE_FILE_API);
        for (int i = 0; i < ARRAY_SIZE(expected); i++)
                TEST_ASSERT_EQUAL_STRING(
                        expected[i], io_fgets(out, sizeof(out), io));
        TEST_ASSERT_NULL(io_fgets(out, sizeof(out), io));
        TEST_ASSERT_EQUAL_INT(expected[4][0], out[0]);
        fclose(fp);
}
```
