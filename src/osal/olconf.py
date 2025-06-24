import os

CONSTRAINTS = [
    (
        ['cver', 'iso'],
        ['osal', 'mutex', 'posix']
    ),
    (
        ['cver', 'iso'],
        ['osal', 'sem', 'posix']
    )
]

def join(olvars):
    settings = olvars.settings()
    blddir = os.path.join(settings['build_path'], olvars.cwd())

    olvars.append('cantil_OLCONF', olvars.path())
    olvars.append('cantil_BLDDIRS', blddir)
    olvars.append('cantil_BLDDIRS', os.path.join(blddir, 'none'))
    olvars.append('cantil_BLDDIRS', os.path.join(blddir, 'posix'))
    olvars.append(
        'cantil_OBJS',
        os.path.join(blddir, settings['osal']['mem'], 'mem.o'))
    olvars.append(
        'cantil_OBJS',
        os.path.join(blddir, settings['osal']['mutex'], 'mutex.o'))
    olvars.append(
        'cantil_OBJS',
        os.path.join(blddir, settings['osal']['sem'], 'sem.o'))
    olvars.append(
        'cantil_OBJS',
        os.path.join(blddir, settings['osal']['sys'], 'sys.o'))
    olvars.append(
        'cantil_OBJS',
        os.path.join(blddir, settings['osal']['fstream'], 'fstream.o'))
