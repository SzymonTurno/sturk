import os

def join(olvars):
    olvars.append('cantil_BLDDIRS', olvars.slashify(olvars.cwd()))
    olvars.append('cantil_OBJS', olvars.slashify(olvars.cwd(), 'cirq.o'))
    olvars.append('cantil_OBJS', olvars.slashify(olvars.cwd(), 'dict.o'))
    olvars.append('cantil_OBJS', olvars.slashify(olvars.cwd(), 'list.o'))
    olvars.append('cantil_OBJS', olvars.slashify(olvars.cwd(), 'pool.o'))
    olvars.append('cantil_OBJS', olvars.slashify(olvars.cwd(), 'rbtree.o'))
    olvars.append('cantil_OBJS', olvars.slashify(olvars.cwd(), 'str.o'))
    olvars.append('cantil_OBJS', olvars.slashify(olvars.cwd(), 'waitq.o'))
