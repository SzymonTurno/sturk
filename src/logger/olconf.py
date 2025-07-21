import os

def join(olvars):
    settings = olvars.settings()
    blddir = os.path.join(settings['build_path'], 'src', 'logger')

    if settings['logger']['trace'] == 'on':
        olvars.append('cantil_EXTRA_CFLAGS', '-DCN_LOGGER_EN=1')
    elif settings['logger']['trace'] == 'off':
        olvars.append('cantil_EXTRA_CFLAGS', '-DCN_LOGGER_EN=0')
    else:
        node.fail('Unknown trace mode: ' + settings['logger']['trace'] + '.')

    if settings['logger']['exceptions'] == 'on':
        olvars.append('cantil_EXTRA_CFLAGS', '-DCN_EXCEPTIONS_EN=1')
    elif settings['logger']['exceptions'] == 'off':
        olvars.append('cantil_EXTRA_CFLAGS', '-DCN_EXCEPTIONS_EN=0')
    else:
        olvars.fail(
            'Unknown exceptions mode: ' + settings['logger']['exceptions'] + '.')

    olvars.append('cantil_OLCONF', olvars.slashify(olvars.path()))
    olvars.append('cantil_BLDDIRS', olvars.slashify(blddir))
    olvars.append('cantil_OBJS', olvars.slashify(blddir, 'trace.o'))
    olvars.append('cantil_OBJS', olvars.slashify(blddir, 'sink.o'))
