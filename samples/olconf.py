import os

def join(olvars):
    olvars.append('sample_DIR', olvars.slashify(olvars.acwd()))
    olvars.append('sample_INC', '-I' + olvars.slashify(olvars.acwd()))
    olvars.append('sample_BLDDIR', olvars.slashify(olvars.cwd()))
    olvars.append(
        'sample_OBJS', olvars.slashify(olvars.cwd(), 'single_thread_pubsub.o'))
    olvars.append(
        'sample_OBJS', olvars.slashify(olvars.cwd(), 'multi_thread_pubsub.o'))
    olvars.append('sample_OBJS', olvars.slashify(olvars.cwd(), 'load_api.o'))
