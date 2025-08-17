def join(olvars):
    settings = olvars.settings()

    olvars.append('test_EXTRA_CFLAGS', '-g')
    olvars.append('test_EXTRA_CFLAGS', '-fprofile-arcs')
    olvars.append('test_EXTRA_CFLAGS', '-ftest-coverage')
    olvars.append('test_EXTRA_DIR', olvars.slashify(olvars.acwd()))
    olvars.append('test_EXTRA_BLDDIR', olvars.slashify(olvars.cwd()))
    olvars.append('test_EXTRA_OBJS', olvars.slashify(olvars.cwd(), 'vertegs_tests.o'))
