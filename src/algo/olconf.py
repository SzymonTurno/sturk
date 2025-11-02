import os

def join(olvars):
    olvars.append('sturk_BLDDIRS', olvars.cwd())
    olvars.append('sturk_OBJS', os.path.join(olvars.cwd(), 'dict.o'))
    olvars.append('sturk_OBJS', os.path.join(olvars.cwd(), 'pool.o'))
    olvars.append('sturk_OBJS', os.path.join(olvars.cwd(), 'str.o'))
    olvars.append('sturk_OBJS', os.path.join(olvars.cwd(), 'waitq.o'))
