import os

def join(olvars):
    settings = olvars.settings()
    blddir = os.path.join(settings['build_path'], 'src', 'broker')

    olvars.append('cantil_BLDDIRS', olvars.slashify(blddir))
    olvars.append('cantil_OBJS', olvars.slashify(blddir, 'broker.o'))
