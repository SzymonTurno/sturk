import os

def append_rules(olvars):
    rule = olvars.rule(os.path.join('$(sample_BLDDIR)', '%.o'))

    rule.normal_depend(os.path.join('$(sample_DIR)', '%.c'))
    rule.order_depend('$(sample_BLDDIR)')
    rule.step(
        '$(CC) $(sturk_CFLAGS) $(sample_INC) $(sturk_INC) -c -o $@ $<')

    rule = olvars.rule('$(sample_BLDDIR)')
    rule.step('$(call MKDIR, $@)')

def join(olvars):
    olvars.append('sample_DIR', os.path.join(olvars.acwd()))
    olvars.append('sample_INC', '-I' + os.path.join(olvars.acwd()))
    olvars.append('sample_BLDDIR', os.path.join(olvars.cwd()))
    olvars.append(
        'sample_OBJS', os.path.join(olvars.cwd(), 'single_thread_pubsub.o'))
    olvars.append(
        'sample_OBJS', os.path.join(olvars.cwd(), 'multi_thread_pubsub.o'))
    olvars.append('sample_OBJS', os.path.join(olvars.cwd(), 'load_api.o'))
    append_rules(olvars)
