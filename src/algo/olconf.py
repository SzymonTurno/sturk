import os

def join(olvars):
    olvars.append('cantil_BLDDIRS', os.path.join(olvars.cwd()))
    olvars.append('cantil_OBJS', os.path.join(olvars.cwd(), 'dict.o'))
    olvars.append('cantil_OBJS', os.path.join(olvars.cwd(), 'pool.o'))
    olvars.append('cantil_OBJS', os.path.join(olvars.cwd(), 'rbtree.o'))
    olvars.append('cantil_OBJS', os.path.join(olvars.cwd(), 'str.o'))
    olvars.append('cantil_OBJS', os.path.join(olvars.cwd(), 'waitq.o'))
