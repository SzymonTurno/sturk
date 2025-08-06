import os

def join(olvars):
    settings = olvars.settings()

    olvars.append('cantil_CFLAGS', '-fanalyzer')
    olvars.append('cantil_CFLAGS', '-Wall')
    olvars.append('cantil_CFLAGS', '-Wcast-align')
    olvars.append('cantil_CFLAGS', '-Wconversion')
    olvars.append('cantil_CFLAGS', '-Wdisabled-optimization')
    olvars.append('cantil_CFLAGS', '-Wextra')
    olvars.append('cantil_CFLAGS', '-Wlogical-op')
    olvars.append('cantil_CFLAGS', '-Wmissing-prototypes')
    olvars.append('cantil_CFLAGS', '-Wnested-externs')
    olvars.append('cantil_CFLAGS', '-Wpadded')
    olvars.append('cantil_CFLAGS', '-Wredundant-decls')
    olvars.append('cantil_CFLAGS', '-Wshadow')
    olvars.append('cantil_CFLAGS', '-Wstrict-prototypes')
    olvars.append('cantil_CFLAGS', '-Wswitch-default')
    olvars.append('cantil_CFLAGS', '-Wwrite-strings')
    olvars.append('cantil_CFLAGS', '-DVX_EXCEPT=cn_except')
    olvars.append('cantil_DIR', olvars.slashify(olvars.acwd()))
    olvars.append('cantil_BLDDIR', olvars.slashify(olvars.cwd()))
    olvars.append('cantil_BLDDIRS', olvars.slashify(olvars.cwd()))

    if settings['cver'] == 'gnu':
        olvars.append('cantil_EXTRA_CFLAGS', '-std=gnu11')
    elif settings['cver'] == 'iso':
        olvars.append('cantil_EXTRA_CFLAGS', '-std=c99')
        olvars.append('cantil_EXTRA_CFLAGS', '-pedantic')
    else:
        olvars.fail('Unknown cver: ' + settings['cver'] + '.')

    if settings['build_type'] == 'release':
        olvars.append('cantil_EXTRA_CFLAGS', '-O3')
    elif settings['build_type'] == 'debug':
        olvars.append('cantil_EXTRA_CFLAGS', '-g')
    elif settings['build_type'] == 'coverage':
        olvars.append('cantil_EXTRA_CFLAGS', '-g')
        olvars.append('cantil_EXTRA_CFLAGS', '-fprofile-arcs')
        olvars.append('cantil_EXTRA_CFLAGS', '-ftest-coverage')
    else:
        olvars.fail('Unknown build type: ' + settings['build_type'] + '.')

    olvars.include('algo')
    olvars.include('broker')
    olvars.include('logger')
    olvars.include('osal')
