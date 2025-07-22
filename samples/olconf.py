import os

def join(olvars):
    settings = olvars.settings()
    blddir = os.path.join(settings['build_path'], olvars.cwd())

    olvars.append('sample_DIR', olvars.slashify(olvars.cwd()))
    olvars.append('sample_BLDDIR', olvars.slashify(blddir))
    olvars.append('sample_INC', '-I' + olvars.cwd())
    olvars.append(
        'sample_OBJS', olvars.slashify(blddir, 'single_thread_pubsub.o'))
    olvars.append(
        'sample_OBJS', olvars.slashify(blddir, 'multi_thread_pubsub.o'))
    olvars.append('sample_OBJS', olvars.slashify(blddir, 'load_api.o'))
