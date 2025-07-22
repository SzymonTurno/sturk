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
    blddir = os.path.join(settings['build_path'], 'src', 'osal')

    if platform == 'win32' and settings['osal']['mutex'] == 'posix':
        olvars.fail('POSIX threads are unavailable on win32.')

    if platform == 'win32' and settings['osal']['sem'] == 'posix':
        olvars.fail('POSIX threads are unavailable on win32.')
    olvars.append('cantil_BLDDIRS', olvars.slashify(blddir))
    olvars.append('cantil_BLDDIRS', olvars.slashify(blddir, 'none'))
    olvars.append('cantil_BLDDIRS', olvars.slashify(blddir, 'posix'))
    olvars.append(
        'cantil_OBJS',
        olvars.slashify(blddir, settings['osal']['mem'], 'mem.o'))
    olvars.append(
        'cantil_OBJS',
        olvars.slashify(blddir, settings['osal']['mutex'], 'mutex.o'))
    olvars.append(
        'cantil_OBJS',
        olvars.slashify(blddir, settings['osal']['sem'], 'sem.o'))
    olvars.append(
        'cantil_OBJS',
        olvars.slashify(blddir, settings['osal']['sys'], 'sys.o'))
    olvars.append(
        'cantil_OBJS',
        olvars.slashify(blddir, settings['osal']['fstream'], 'fstream.o'))
