import os

def append_rules(olvars):
    rule = olvars.rule(olvars.slashify('$(sample_BLDDIR)', '%.o'))

    rule.normal_depend(olvars.slashify('$(sample_DIR)', '%.c'))
    rule.order_depend('$(sample_BLDDIR)')
    rule.step(
        '$(CC) $(cantil_CFLAGS) $(sample_INC) $(cantil_INC) -c -o $@ $<')

    rule = olvars.rule('$(sample_BLDDIR)')
    rule.step('$(call MKDIR, $@)')

def join(olvars):
    olvars.append('sample_DIR', olvars.slashify(olvars.acwd()))
    olvars.append('sample_INC', '-I' + olvars.slashify(olvars.acwd()))
    olvars.append('sample_BLDDIR', olvars.slashify(olvars.cwd()))
    olvars.append(
        'sample_OBJS', olvars.slashify(olvars.cwd(), 'single_thread_pubsub.o'))
    olvars.append(
        'sample_OBJS', olvars.slashify(olvars.cwd(), 'multi_thread_pubsub.o'))
    olvars.append('sample_OBJS', olvars.slashify(olvars.cwd(), 'load_api.o'))
    append_rules(olvars)
