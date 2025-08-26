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

    if platform == 'win32' and settings['osal']['mutex'] == 'posix':
        olvars.fail('POSIX threads are unavailable on win32.')

    if platform == 'win32' and settings['osal']['sem'] == 'posix':
        olvars.fail('POSIX threads are unavailable on win32.')
    olvars.append('cantil_BLDDIRS', olvars.cwd())
    olvars.append('cantil_BLDDIRS', os.path.join(olvars.cwd(), 'none'))
    olvars.append('cantil_BLDDIRS', os.path.join(olvars.cwd(), 'posix'))
    olvars.append(
        'cantil_OBJS',
        os.path.join(olvars.cwd(), settings['osal']['mem'], 'mem.o'))
    olvars.append(
        'cantil_OBJS',
        os.path.join(olvars.cwd(), settings['osal']['mutex'], 'mutex.o'))
    olvars.append(
        'cantil_OBJS',
        os.path.join(olvars.cwd(), settings['osal']['sem'], 'sem.o'))
    olvars.append(
        'cantil_OBJS',
        os.path.join(olvars.cwd(), settings['osal']['sys'], 'sys.o'))
    olvars.append(
        'cantil_OBJS',
        os.path.join(olvars.cwd(), settings['osal']['fstream'], 'fstream.o'))
