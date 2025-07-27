import os

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

def join(olvars):
    settings = olvars.settings()

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
