import os

CONSTRAINTS = [
    (
        ['cver', 'iso'],
        ['test', 'threads', 'on']
    )
]

def fixture_dir():
    return os.path.join('Unity', 'extras', 'fixture', 'src')

def memory_dir():
    return os.path.join('Unity', 'extras', 'memory', 'src')

def join(olvars):
    settings = olvars.settings()
    blddir = os.path.join(settings['build_path'], olvars.cwd())

    if settings['test']['threads'] == 'on':
        olvars.append('test_CFLAGS', '-DTHREADS_EN=1')
    elif settings['test']['threads'] == 'off':
        olvars.append('test_CFLAGS', '-DTHREADS_EN=0')
    else:
        node.fail('Unknown threads mode: ' + settings['test']['threads'] + '.')

    olvars.append('cantil_OLCONF', olvars.path())
    olvars.append('unity_DIR', 'Unity')
    olvars.append('unity_BLDDIR', os.path.join(blddir, 'Unity'))
    olvars.append('unity_BLDDIRS', os.path.join(blddir, 'Unity'))
    olvars.append('unity_BLDDIRS', os.path.join(blddir, 'Unity', 'src'))
    olvars.append('unity_BLDDIRS', os.path.join(blddir, fixture_dir()))
    olvars.append('unity_BLDDIRS', os.path.join(blddir, memory_dir()))
    olvars.append('unity_INCS', '-I' + os.path.join('Unity', 'src'))
    olvars.append('unity_INCS', '-I' + fixture_dir())
    olvars.append('unity_INCS', '-I' + memory_dir())
    olvars.append('unity_OBJS', os.path.join(blddir, 'Unity', 'src', 'unity.o'))
    olvars.append('unity_OBJS', os.path.join(blddir, fixture_dir(), 'unity_fixture.o'))
    olvars.append('unity_OBJS', os.path.join(blddir, memory_dir(), 'unity_memory.o'))
    olvars.append('test_DIR', olvars.cwd())
    olvars.append('test_BLDDIR', blddir)
    olvars.append('test_OBJS', os.path.join(blddir, 'main.o'))
    olvars.append('test_DEPS', '$(test_OBJS)')
    olvars.append('test_DEPS', '$(unity_OBJS)')
    olvars.append('test_DEPS', '$(sample_OBJS)')
    olvars.append('test_DEPS', '$(cantil_BLDDIR)/libcantil.a')
