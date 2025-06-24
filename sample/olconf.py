import os

def join(olvars):
    settings = olvars.settings()
    blddir = os.path.join(settings['build_path'], olvars.cwd())

    olvars.append('cantil_OLCONF', olvars.path())
    olvars.append('sample_DIR', olvars.cwd())
    olvars.append('sample_BLDDIR', blddir)
    olvars.append('sample_INC', '-I' + olvars.cwd())
    olvars.append('sample_OBJS', os.path.join(blddir, 'single_thread_pubsub.o'))
    olvars.append('sample_OBJS', os.path.join(blddir, 'multi_thread_pubsub.o'))
