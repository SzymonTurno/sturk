# Logger

The logger writes to all attached IOs. IO can be a memory buffer, file, device.
For information on how to use IO interface
see [basis](https://szymonturno.github.io/sturk/md_src_2basis_2README.html).

## Logger example

```c
TEST(logger, should_write_to_memory_buffer)
{
        StIoBuffer buff[iobuffer_calclen(32)] = {0};
        StIo* io = io_init(buff);

        logger_attach(INFO, io);
        trace(INFO, "Alice", "%x", 0xDEADBEEF);
        io_putc(io, '\0');
        TEST_ASSERT_EQUAL_STRING(
                "[info][Alice] deadbeef\n", iobuffer_front(buff));
        logger_detach(INFO, io);
}
```
