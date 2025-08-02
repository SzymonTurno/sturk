import os

def join(olvars):
    settings = olvars.settings()

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

    olvars.append('cantil_BLDDIRS', olvars.slashify(olvars.cwd()))
    olvars.append('cantil_OBJS', olvars.slashify(olvars.cwd(), 'trace.o'))
    olvars.append('cantil_OBJS', olvars.slashify(olvars.cwd(), 'streambag.o'))
