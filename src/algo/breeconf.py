import os
from cydiom.cycfg import Cynode

def create(params):
    node = Cynode(params)

    node.objs.append('binode.o')
    node.objs.append('list.o')
    node.objs.append('pool.o')
    node.objs.append('rbtree.o')
    node.objs.append('strnode.o')
    node.objs.append('waitq.o')
    return node
