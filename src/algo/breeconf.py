import os
from cantil.cncfg import Canode

def create(params):
    node = Canode(params)

    node.objs.append('binode.o')
    node.objs.append('list.o')
    node.objs.append('str.o')
    node.objs.append('pool.o')
    node.objs.append('rbtree.o')
    node.objs.append('strnode.o')
    node.objs.append('waitq.o')
    return node
