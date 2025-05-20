import os
from cydiom.cycfg import Cynode

def create(params):
    node = Cynode(params)
    osal = node.settings()['cydiom']['osal']

    node.objs.append(os.path.join(osal['mem'], 'mem.o'))
    node.objs.append(os.path.join(osal['mutex'], 'mutex.o'))
    node.objs.append(os.path.join(osal['sem'], 'sem.o'))
    node.objs.append(os.path.join(osal['sysio'], 'sysio.o'))
    return node
