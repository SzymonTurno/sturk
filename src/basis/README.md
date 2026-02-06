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
 * @struct StIo
 *
 * @brief Vtable for IO operations.
 *
 * Use this struct as the first member of a compound type and set its callbacks
 * to implement the IO interface.
 */
struct StIo {
        /**
         * @var void (*putc_cb)(struct StIo*, char)
         *
         * @brief Callback for writing a single char.
         */
        void (*putc_cb)(struct StIo*, char);

        /**
         * @var char (*getc_cb)(struct StIo*)
         *
         * @brief Callback for reading a single char.
         */
        char (*getc_cb)(struct StIo*);
};
```

### Memory buffer example

```c
TEST(basis, should_write_to_memory_buffer)
{
        char out[16] = {0};

        /* Allocate sixteen bytes plus metadata for the memory buffer. */
        struct St io[iocontig_calclen(sizeof(out))] = {0};

        /* Initialize the memory buffer. */
        iobuffer_init((struct StIoBuffer*)io);

        /* Print "deadbeef" to the memory buffer. */
        io_print(io, "%x", 0xDEADBEEF);

        /* Put EOF character in the memory buffer. */
        io_putc(io, IO_EOF);

        /* Initialize the memory buffer again to read from the beginning. */
        iobuffer_init((struct StIoBuffer*)io);

        /* Read the buffer. */
        TEST_ASSERT_EQUAL_STRING("deadbeef", io_fgets(out, sizeof(out), io));
}
```

### Reading from a file

#### Implement the IO interface

```c
struct IoFile {
        struct StIo vt;
        FILE* fp;
};

static void file_putc(struct StIo* vp, char c)
{
        struct IoFile* self = (struct IoFile*)vp;

        putc(c, self->fp);
}

static char file_getc(struct StIo* vp)
{
        struct IoFile* self = (struct IoFile*)vp;

        return (char)getc(self->fp);
}

struct StIo* iofile_create(FILE* fp)
{
        struct IoFile* file = ST_NEW(struct IoFile);
        struct StIo* ret = (struct StIo*)file;

        /* Make sure that `vt` is the first member. */
        ASSERT(ret == &file->vt);
        ret->putc_cb = file_putc;
        ret->getc_cb = file_getc;
        file->fp = fp;
        return ret;
}
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
        FILE* fp = fopen(LOREM_TXT, "r");
        struct StIo* io = iofile_create(fp);

        for (int i = 0; i < ARRAY_SIZE(expected); i++)
                TEST_ASSERT_EQUAL_STRING(
                        expected[i], io_fgets(out, sizeof(out), io));
        TEST_ASSERT_NULL(io_fgets(out, sizeof(out), io));
        TEST_ASSERT_EQUAL_INT(expected[4][0], out[0]);
        st_free(io);
        fclose(fp);
}
```
