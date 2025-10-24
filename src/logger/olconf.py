import os

def join(olvars):
    settings = olvars.settings()

    if settings['logger']['tracking']:
        olvars.append('sturk_EXTRA_CFLAGS', '-DST_TRACKING=1')
    else:
        olvars.append('sturk_EXTRA_CFLAGS', '-DST_TRACKING=0')

    olvars.append('sturk_BLDDIRS', os.path.join(olvars.cwd()))
    olvars.append('sturk_OBJS', os.path.join(olvars.cwd(), 'iobag.o'))
    olvars.append('sturk_OBJS', os.path.join(olvars.cwd(), 'logger.o'))
