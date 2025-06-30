import os

def join(olvars):
    settings = olvars.settings()
    blddir = os.path.join(settings['build_path'], olvars.cwd())

    olvars.append('cantil_OLCONF', olvars.unix(olvars.path()))
    olvars.append('sample_DIR', olvars.unix(olvars.cwd()))
    olvars.append('sample_BLDDIR', olvars.unix(blddir))
    olvars.append('sample_INC', '-I' + olvars.cwd())
    olvars.append('sample_OBJS', olvars.unixjoin(blddir, 'single_thread_pubsub.o'))
    olvars.append('sample_OBJS', olvars.unixjoin(blddir, 'multi_thread_pubsub.o'))
    olvars.append('sample_OBJS', olvars.unixjoin(blddir, 'load_api.o'))
