import os
from cantil.cncfg import Canode

def create(params):
    node = Canode(params)
    settings = node.settings()['cantil']['logger']

    if settings['log'] == 'on':
        node.cflags.append('-DCN_LOG_EN=1')
    elif settings['log'] == 'off':
        node.cflags.append('-DCN_LOG_EN=0')
    else:
        node.fail('Unknown log mode: ' + settings['log'] + '.')

    if settings['exceptions'] == 'on':
        node.cflags.append('-DCN_EXCEPTIONS_EN=1')
    elif settings['exceptions'] == 'off':
        node.cflags.append('-DCN_EXCEPTIONS_EN=0')
    else:
        node.fail('Unknown exceptions mode: ' + settings['exceptions'] + '.')

    node.objs.append('log.o')
    node.objs.append('sink.o')
    return node
