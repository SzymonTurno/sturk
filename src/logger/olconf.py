import os
from cantil.cncfg import Canode

def create(params):
    node = Canode(params)
    settings = node.settings()['cantil']['logger']

    if settings['trace'] == 'on':
        node.cflags.append('-DCN_LOGGER_EN=1')
    elif settings['trace'] == 'off':
        node.cflags.append('-DCN_LOGGER_EN=0')
    else:
        node.fail('Unknown trace mode: ' + settings['trace'] + '.')

    if settings['exceptions'] == 'on':
        node.cflags.append('-DCN_EXCEPTIONS_EN=1')
    elif settings['exceptions'] == 'off':
        node.cflags.append('-DCN_EXCEPTIONS_EN=0')
    else:
        node.fail('Unknown exceptions mode: ' + settings['exceptions'] + '.')

    node.objs.append('trace.o')
    node.objs.append('sink.o')
    return node
