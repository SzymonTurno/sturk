import os

def append_rules(olvars):
    rule = olvars.rule(os.path.join('$(test_EXTRA_BLDDIR)', '%.o'))

    rule.normal_depend(os.path.join('$(test_EXTRA_DIR)', '%.c'))
    rule.order_depend('$(test_EXTRA_BLDDIR)')
    rule.step(
        '$(CC) $(test_CFLAGS) $(test_EXTRA_CFLAGS) $(cantil_INC) -c -o $@ $<')

    rule = olvars.rule('$(test_EXTRA_BLDDIR)')
    rule.normal_depend('$(test_BLDDIR)')
    rule.step('$(call MKDIR, $@)')

def join(olvars):
    settings = olvars.settings()

    olvars.append('test_EXTRA_CFLAGS', '-g')
    olvars.append('test_EXTRA_CFLAGS', '-fprofile-arcs')
    olvars.append('test_EXTRA_CFLAGS', '-ftest-coverage')
    olvars.append('test_EXTRA_DIR', os.path.join(olvars.acwd()))
    olvars.append('test_EXTRA_BLDDIR', os.path.join(olvars.cwd()))
    olvars.append(
        'test_EXTRA_OBJS', os.path.join(olvars.cwd(), 'vertegs_tests.o'))
    append_rules(olvars)
