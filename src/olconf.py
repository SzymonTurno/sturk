import os

def append_rules(olvars):
    rule = olvars.rule(os.path.join('$(sturk_BLDDIR)', 'libsturk.a'))

    rule.normal_depend('$(sturk_OBJS)')
    rule.step('ar rcs $@ $(sturk_OBJS)')

    rule = olvars.rule(os.path.join('$(sturk_BLDDIR)', '%.o'))
    rule.normal_depend(os.path.join('$(sturk_DIR)', '%.c'))
    rule.order_depend('$(sturk_BLDDIRS)')
    rule.step('$(CC) $(sturk_CFLAGS) $(sturk_EXTRA_CFLAGS) $(sturk_INC) -c -o $@ $<')

    rule = olvars.rule('$(sturk_BLDDIRS)')
    rule.step('$(call MKDIR, $@)')

def join(olvars):
    settings = olvars.settings()

    olvars.append('sturk_CFLAGS', '-fanalyzer')
    olvars.append('sturk_CFLAGS', '-Wall')
    olvars.append('sturk_CFLAGS', '-Wcast-align')
    olvars.append('sturk_CFLAGS', '-Wconversion')
    olvars.append('sturk_CFLAGS', '-Wdisabled-optimization')
    olvars.append('sturk_CFLAGS', '-Wextra')
    olvars.append('sturk_CFLAGS', '-Wlogical-op')
    olvars.append('sturk_CFLAGS', '-Wmissing-prototypes')
    olvars.append('sturk_CFLAGS', '-Wnested-externs')
    olvars.append('sturk_CFLAGS', '-Wpadded')
    olvars.append('sturk_CFLAGS', '-Wredundant-decls')
    olvars.append('sturk_CFLAGS', '-Wshadow')
    olvars.append('sturk_CFLAGS', '-Wstrict-prototypes')
    olvars.append('sturk_CFLAGS', '-Wswitch-default')
    olvars.append('sturk_CFLAGS', '-Wwrite-strings')
    olvars.append('sturk_CFLAGS', '-DVX_EXCEPT=st_except')
    olvars.append('sturk_DIR', os.path.join(olvars.acwd()))
    olvars.append('sturk_BLDDIR', os.path.join(olvars.cwd()))
    olvars.append('sturk_BLDDIRS', os.path.join(olvars.cwd()))

    if settings['cver'] == 'gnu':
        olvars.append('sturk_EXTRA_CFLAGS', '-std=gnu11')
    elif settings['cver'] == 'iso':
        olvars.append('sturk_EXTRA_CFLAGS', '-std=c99')
        olvars.append('sturk_EXTRA_CFLAGS', '-pedantic')
    else:
        olvars.fail('Unknown cver: ' + settings['cver'] + '.')

    if settings['build_type'] == 'release':
        olvars.append('sturk_EXTRA_CFLAGS', '-O3')
    elif settings['build_type'] == 'debug':
        olvars.append('sturk_EXTRA_CFLAGS', '-g')
    elif settings['build_type'] == 'coverage':
        olvars.append('sturk_EXTRA_CFLAGS', '-g')
        olvars.append('sturk_EXTRA_CFLAGS', '-fprofile-arcs')
        olvars.append('sturk_EXTRA_CFLAGS', '-ftest-coverage')
    else:
        olvars.fail('Unknown build type: ' + settings['build_type'] + '.')

    olvars.include('algo')
    olvars.include('broker')
    olvars.include('logger')
    olvars.include('osal')
    append_rules(olvars)
