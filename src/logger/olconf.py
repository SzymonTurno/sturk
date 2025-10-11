import os

def join(olvars):
    settings = olvars.settings()

    if settings['logger']['tracing']:
        olvars.append('sturk_EXTRA_CFLAGS', '-DST_LOGGER_EN=1')
    else:
        olvars.append('sturk_EXTRA_CFLAGS', '-DST_LOGGER_EN=0')

    if settings['logger']['excepting']:
        olvars.append('sturk_EXTRA_CFLAGS', '-DST_EXCEPTIONS_EN=1')
    else:
        olvars.append('sturk_EXTRA_CFLAGS', '-DST_EXCEPTIONS_EN=0')

    olvars.append('sturk_BLDDIRS', os.path.join(olvars.cwd()))
    olvars.append('sturk_OBJS', os.path.join(olvars.cwd(), 'iobag.o'))
    olvars.append('sturk_OBJS', os.path.join(olvars.cwd(), 'logger.o'))
