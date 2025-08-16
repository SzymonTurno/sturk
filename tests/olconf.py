import os
import platform

CONSTRAINTS = [
    (
        ['cver', 'iso'],
        ['test', 'threads', 'on']
    )
]

def fixture_dir():
    return os.path.join('Unity', 'extras', 'fixture')

def memory_dir():
    return os.path.join('Unity', 'extras', 'memory')

def append_rules(olvars):
    rule = olvars.rule(olvars.slashify('$(test_BLDDIR)', 'reports.d', 'coverage.d'))
    dep = olvars.slashify('$(test_BLDDIR)', 'reports.d', 'coverage.info')
    tmp = ''

    rule.normal_depend(dep)
    if platform == 'win32':
        rule.step('$(call MKDIR, $@)')
    else:
        rule.step('genhtml ' + dep + ' --show-details --output-directory $@')

    rule = olvars.rule(olvars.slashify('$(test_BLDDIR)', 'reports.d', 'coverage.info'))
    dep = olvars.slashify('$(test_BLDDIR)', 'reports.d', 'valgrind.info')
    rule.normal_depend(dep)
    if platform == 'win32':
        rule.step('echo No coverage for Windows. > $@')
    else:
        rule.step('lcov --capture --directory ./ --output-file $@')

    rule = olvars.rule(olvars.slashify('$(test_BLDDIR)', 'reports.d', 'valgrind.info'))
    dep = olvars.slashify('$(test_BLDDIR)', 'app')
    rule.normal_depend(dep)
    dep = olvars.slashify('$(test_BLDDIR)', 'reports.d')
    rule.order_depend(dep)
    if platform == 'win32':
        rule.step(olvars.slashify('.', '$(test_BLDDIR)', 'app'))
        rule.step('echo No memchecks for Windows. > $@')
    else:
        tmp = 'valgrind --leak-check=full --show-leak-kinds=all '
        tmp = tmp + '--track-origins=yes --error-exitcode=1 --log-file="$@" '
        tmp = tmp + olvars.slashify('.', '$(test_BLDDIR)', 'app')
        rule.step(tmp)

    rule = olvars.rule(olvars.slashify('$(test_BLDDIR)', 'reports.d'))
    rule.step('$(call MKDIR, $@)')

    rule = olvars.rule(olvars.slashify('$(test_BLDDIR)', 'app'))
    tmp = '$(test_OBJS) $(test_EXTRA_OBJS) $(unity_OBJS) $(sample_OBJS) '
    tmp = tmp + olvars.slashify('$(cantil_BLDDIR)', 'libcantil.a')
    rule.normal_depend(tmp)
    rule.step('$(CC) -o $@ ' + tmp + ' -lgcov --coverage')

    rule = olvars.rule(olvars.slashify('$(test_BLDDIR)', 'main.o'))
    rule.normal_depend(olvars.slashify('$(test_DIR)', 'main.c'))
    rule.order_depend('$(test_EXTRA_OBJS) $(unity_DIR)')
    rule.step('$(CC) $(test_CFLAGS) $(unity_INCS) $(sample_INC) $(cantil_INC) -c -o $@ $<')

    rule = olvars.rule('$(test_BLDDIR)')
    rule.step('$(call MKDIR, $@)')

    rule = olvars.rule(olvars.slashify('$(unity_DIR)', '%.o'))
    rule.normal_depend(olvars.slashify('$(unity_DIR)', '%.c'))
    rule.step('$(CC) $(unity_INCS) -c -o $@ $<')

    rule = olvars.rule(olvars.slashify('$(unity_DIR)', '%.c'))
    rule.normal_depend('$(unity_DIR)')
    rule.step('cd $(unity_DIR) && git reset --hard v2.6.1')

    rule = olvars.rule('$(unity_DIR)')
    rule.step('git clone https://github.com/ThrowTheSwitch/Unity.git')
    rule.step('cd $(unity_DIR) && git reset --hard v2.6.1')

def join(olvars):
    settings = olvars.settings()

    if settings['cver'] == 'gnu':
        olvars.append('test_CFLAGS', '-std=gnu11')
    elif settings['cver'] == 'iso':
        olvars.append('test_CFLAGS', '-std=c99')
        olvars.append('test_CFLAGS', '-pedantic')
    else:
        olvars.fail('Unknown cver: ' + settings['cver'] + '.')

    if settings['test']['threads'] == 'on':
        olvars.append('test_CFLAGS', '-DTHREADS_EN=1')
    elif settings['test']['threads'] == 'off':
        olvars.append('test_CFLAGS', '-DTHREADS_EN=0')
    else:
        node.fail('Unknown threads mode: ' + settings['test']['threads'] + '.')

    olvars.append('unity_DIR', 'Unity')
    olvars.append('unity_INCS', '-I' + olvars.slashify('Unity', 'src'))
    olvars.append('unity_INCS', '-I' + olvars.slashify(fixture_dir(), 'src'))
    olvars.append('unity_INCS', '-I' + olvars.slashify(memory_dir(), 'src'))
    olvars.append('unity_OBJS', olvars.slashify('Unity', 'src', 'unity.o'))
    olvars.append(
        'unity_OBJS',
        olvars.slashify(fixture_dir(), 'src', 'unity_fixture.o'))
    olvars.append(
        'unity_OBJS',
        olvars.slashify(memory_dir(), 'src', 'unity_memory.o'))
    olvars.append('test_DIR', olvars.slashify(olvars.acwd()))
    olvars.append('test_BLDDIR', olvars.cwd())
    olvars.append('test_OBJS', olvars.slashify(olvars.cwd(), 'main.o'))
    olvars.include('extra')
    append_rules(olvars)
