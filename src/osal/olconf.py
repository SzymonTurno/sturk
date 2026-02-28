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
    ),
    (
        ['hosted', False],
        ['osal', 'mem', 'std']
    ),
    (
        ['hosted', False],
        ['osal', 'mutex', 'posix']
    ),
    (
        ['hosted', False],
        ['osal', 'sys', 'std']
    ),
]

def join_external_memfile(olvars):
    sturk_path = olvars.acwd().split()[0].split()[0]
    tmp = sturk_path
    relpath = olvars.settings()['relative_path']

    while os.path.join(tmp, relpath, 'src', 'osal') != olvars.acwd():
        tmp = tmp.split()[0]

    olvars.append(
        'sturk_OBJS',
        os.path.join(tmp, olvars.settings()['osal_external']['mem']) + '.o'
    )

def join(olvars):
    settings = olvars.settings()

    if platform == 'win32' and settings['osal']['mutex'] == 'posix':
        olvars.fail('POSIX threads are unavailable on win32.')

    if platform == 'win32' and settings['osal']['sem'] == 'posix':
        olvars.fail('POSIX threads are unavailable on win32.')
    olvars.append('sturk_BLDDIRS', olvars.cwd())
    olvars.append('sturk_BLDDIRS', os.path.join(olvars.cwd(), 'mem'))
    olvars.append('sturk_BLDDIRS', os.path.join(olvars.cwd(), 'mutex'))
    olvars.append('sturk_BLDDIRS', os.path.join(olvars.cwd(), 'sem'))
    olvars.append('sturk_BLDDIRS', os.path.join(olvars.cwd(), 'sys'))
    if settings['osal_external']['mem'] != "":
        join_external_memfile(olvars)
    else:
        olvars.append(
            'sturk_OBJS',
            os.path.join(olvars.cwd(), 'mem', settings['osal']['mem'] + '.o')
        )

    olvars.append(
        'sturk_OBJS',
        os.path.join(olvars.cwd(), 'mutex', settings['osal']['mutex'] + '.o')
    )
    olvars.append(
        'sturk_OBJS',
        os.path.join(olvars.cwd(), 'sem', settings['osal']['sem'] + '.o')
    )
    olvars.append(
        'sturk_OBJS',
        os.path.join(olvars.cwd(), 'sys', settings['osal']['sys'] + '.o')
    )
