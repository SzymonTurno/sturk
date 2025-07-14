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
    blddir = os.path.join(settings['build_path'], olvars.cwd())

    if settings['test']['threads'] == 'on':
        olvars.append('test_CFLAGS', '-DTHREADS_EN=1')
    elif settings['test']['threads'] == 'off':
        olvars.append('test_CFLAGS', '-DTHREADS_EN=0')
    else:
        node.fail('Unknown threads mode: ' + settings['test']['threads'] + '.')

    olvars.append('cantil_OLCONF', olvars.slashify(olvars.path()))
    olvars.append('unity_DIR', 'Unity')
    olvars.append('unity_BLDDIR', olvars.slashify(blddir, 'Unity'))
    olvars.append('unity_BLDDIRS', olvars.slashify(blddir, 'Unity'))
    olvars.append('unity_BLDDIRS', olvars.slashify(blddir, 'Unity', 'src'))
    olvars.append('unity_BLDDIRS', olvars.slashify(blddir, 'Unity', 'extras'))
    olvars.append('unity_BLDDIRS', olvars.slashify(blddir, fixture_dir()))
    olvars.append(
        'unity_BLDDIRS', olvars.slashify(blddir, fixture_dir(), 'src'))
    olvars.append('unity_BLDDIRS', olvars.slashify(blddir, memory_dir()))
    olvars.append('unity_BLDDIRS', olvars.slashify(blddir, memory_dir(), 'src'))
    olvars.append('unity_INCS', '-I' + olvars.slashify('Unity', 'src'))
    olvars.append('unity_INCS', '-I' + olvars.slashify(fixture_dir(), 'src'))
    olvars.append('unity_INCS', '-I' + olvars.slashify(memory_dir(), 'src'))
    olvars.append(
        'unity_OBJS', olvars.slashify(blddir, 'Unity', 'src', 'unity.o'))
    olvars.append(
        'unity_OBJS',
        olvars.slashify(blddir, fixture_dir(), 'src', 'unity_fixture.o'))
    olvars.append(
        'unity_OBJS',
        olvars.slashify(blddir, memory_dir(), 'src', 'unity_memory.o'))
    olvars.append('test_DIR', olvars.slashify(olvars.cwd()))
    olvars.append('test_BLDDIR', olvars.slashify(blddir))
    olvars.append('test_OBJS', olvars.slashify(blddir, 'main.o'))
    olvars.append('test_DEPS', '$(test_OBJS)')
    olvars.append('test_DEPS', '$(unity_OBJS)')
    olvars.append('test_DEPS', '$(sample_OBJS)')
    olvars.append(
        'test_DEPS', olvars.slashify('$(cantil_BLDDIR)', 'libcantil.a'))
