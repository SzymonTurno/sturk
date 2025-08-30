import os

def join(olvars):
    olvars.append('sturk_BLDDIRS', olvars.cwd())
    olvars.append('sturk_OBJS', os.path.join(olvars.cwd(), 'arena.o'))
