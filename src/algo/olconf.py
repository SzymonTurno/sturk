import os

def join(olvars):
    settings = olvars.settings()
    blddir = os.path.join(settings['build_path'], olvars.cwd())

    olvars.append('cantil_OLCONF', olvars.unix(olvars.path()))
    olvars.append('cantil_BLDDIRS', olvars.unix(blddir))
    olvars.append('cantil_OBJS', olvars.unixjoin(blddir, 'cirq.o'))
    olvars.append('cantil_OBJS', olvars.unixjoin(blddir, 'dict.o'))
    olvars.append('cantil_OBJS', olvars.unixjoin(blddir, 'list.o'))
    olvars.append('cantil_OBJS', olvars.unixjoin(blddir, 'pool.o'))
    olvars.append('cantil_OBJS', olvars.unixjoin(blddir, 'rbtree.o'))
    olvars.append('cantil_OBJS', olvars.unixjoin(blddir, 'str.o'))
    olvars.append('cantil_OBJS', olvars.unixjoin(blddir, 'waitq.o'))
