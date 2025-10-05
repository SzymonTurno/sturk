import os

def join(olvars):
    olvars.append('sturk_BLDDIRS', olvars.cwd())
    olvars.append('sturk_OBJS', os.path.join(olvars.cwd(), 'arena.o'))
    olvars.append('sturk_OBJS', os.path.join(olvars.cwd(), 'io.o'))
    olvars.append('sturk_OBJS', os.path.join(olvars.cwd(), 'ioprint.o'))
