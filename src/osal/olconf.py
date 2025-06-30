import os
from sys import platform

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

    if platform == 'win32' and settings['osal']['mutex'] == 'posix':
        olvars.fail('POSIX threads are unavailable on win32.')

    if platform == 'win32' and settings['osal']['sem'] == 'posix':
        olvars.fail('POSIX threads are unavailable on win32.')
    olvars.append('cantil_OLCONF', olvars.unix(olvars.path()))
    olvars.append('cantil_BLDDIRS', olvars.unix(blddir))
    olvars.append('cantil_BLDDIRS', olvars.unixjoin(blddir, 'none'))
    olvars.append('cantil_BLDDIRS', olvars.unixjoin(blddir, 'posix'))
    olvars.append(
        'cantil_OBJS',
        olvars.unixjoin(blddir, settings['osal']['mem'], 'mem.o'))
    olvars.append(
        'cantil_OBJS',
        olvars.unixjoin(blddir, settings['osal']['mutex'], 'mutex.o'))
    olvars.append(
        'cantil_OBJS',
        olvars.unixjoin(blddir, settings['osal']['sem'], 'sem.o'))
    olvars.append(
        'cantil_OBJS',
        olvars.unixjoin(blddir, settings['osal']['sys'], 'sys.o'))
    olvars.append(
        'cantil_OBJS',
        olvars.unixjoin(blddir, settings['osal']['fstream'], 'fstream.o'))
