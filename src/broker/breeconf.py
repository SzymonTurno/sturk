import os
from cydiom.cycfg import Cynode

def create(params):
    node = Cynode(params)

    node.objs.append('broker.o')
    return node
