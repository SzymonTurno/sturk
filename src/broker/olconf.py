import os

def join(olvars):
    settings = olvars.settings()
    blddir = os.path.join(settings['build_path'], olvars.cwd())

    olvars.append('cantil_OLCONF', olvars.path())
    olvars.append('cantil_BLDDIRS', blddir)
    olvars.append('cantil_OBJS', os.path.join(blddir, 'broker.o'))
