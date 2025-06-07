import os
from cantil.cncfg import Canode

def create(params):
    node = Canode(params)

    node.objs.append('cirq.o')
    node.objs.append('dict.o')
    node.objs.append('list.o')
    node.objs.append('pool.o')
    node.objs.append('rbtree.o')
    node.objs.append('str.o')
    node.objs.append('waitq.o')
    return node
