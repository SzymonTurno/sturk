import os
from cydiom.cycfg import Cynode

def create(params):
    node = Cynode(params)
    settings = node.settings()['cydiom']['debug']

    if settings['log'] == 'on':
        node.cflags.append('-DUB_LOG_EN=1')
    elif settings['log'] == 'off':
        node.cflags.append('-DUB_LOG_EN=0')
    else:
        node.fail('Unknown log mode: ' + settings['log'] + '.')

    if settings['exceptions'] == 'on':
        node.cflags.append('-DUB_EXCEPTIONS_EN=1')
    elif settings['exceptions'] == 'off':
        node.cflags.append('-DUB_EXCEPTIONS_EN=0')
    else:
        node.fail('Unknown exceptions mode: ' + settings['exceptions'] + '.')

    node.objs.append('logger.o')
    node.objs.append('log.o')
    node.objs.append('snapshot.o')
    return node
