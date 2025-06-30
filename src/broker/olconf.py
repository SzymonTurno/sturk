import os

def join(olvars):
    settings = olvars.settings()
    blddir = os.path.join(settings['build_path'], olvars.cwd())

    olvars.append('cantil_OLCONF', olvars.unix(olvars.path()))
    olvars.append('cantil_BLDDIRS', olvars.unix(blddir))
    olvars.append('cantil_OBJS', olvars.unixjoin(blddir, 'broker.o'))
