import os
from cantil.cncfg import Canode

def create(params):
    node = Canode(params)

    node.objs.append('broker.o')
    return node
