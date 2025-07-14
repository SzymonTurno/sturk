import os

def join(olvars):
    settings = olvars.settings()
    blddir = os.path.join(settings['build_path'], olvars.cwd())

    olvars.append('cantil_OLCONF', olvars.slashify(olvars.path()))
    olvars.append('cantil_BLDDIRS', olvars.slashify(blddir))
    olvars.append('cantil_OBJS', olvars.slashify(blddir, 'cirq.o'))
    olvars.append('cantil_OBJS', olvars.slashify(blddir, 'dict.o'))
    olvars.append('cantil_OBJS', olvars.slashify(blddir, 'list.o'))
    olvars.append('cantil_OBJS', olvars.slashify(blddir, 'pool.o'))
    olvars.append('cantil_OBJS', olvars.slashify(blddir, 'rbtree.o'))
    olvars.append('cantil_OBJS', olvars.slashify(blddir, 'str.o'))
    olvars.append('cantil_OBJS', olvars.slashify(blddir, 'waitq.o'))
