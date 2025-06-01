import os
from cantil.cncfg import Canode

def create(params):
    node = Canode(params)
    osal = node.settings()['cantil']['osal']

    node.objs.append(os.path.join(osal['mem'], 'mem.o'))
    node.objs.append(os.path.join(osal['mutex'], 'mutex.o'))
    node.objs.append(os.path.join(osal['sem'], 'sem.o'))
    node.objs.append(os.path.join(osal['sys'], 'sys.o'))
    node.objs.append(os.path.join(osal['fstream'], 'fstream.o'))
    return node
