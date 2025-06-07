import os
import sys

sys.path.append(os.path.join(os.path.dirname(sys.argv[0]), 'scripts'))

from cantil import olgite
from cantil import cncfg

def create(params):
    node = cncfg.Canode(params)
    settings = node.settings()['cantil']

    if settings['cver'] == 'gnu':
        node.cflags.append('-std=gnu11')
    elif settings['cver'] == 'iso':
        node.cflags.append('-std=c99 -pedantic')
    else:
        node.fail('Unknown cver: ' + settings['cver'] + '.')

    if settings['build_type'] == 'release':
        node.cflags.append('-O3')
    elif settings['build_type'] == 'debug':
        node.cflags.append('-g')
    elif settings['build_type'] == 'coverage':
        node.cflags.append('-g -fprofile-arcs -ftest-coverage')
    else:
        node.fail('Unknown build type: ' + settings['build_type'] + '.')

    node.append_constraint(
        ['cantil', 'cver', 'iso'],
        ['cantil', 'osal', 'mutex', 'posix']
    )
    node.include('algo')
    node.include('broker')
    node.include('logger')
    node.include('osal')
    return node

def main():
    srcdir = os.path.dirname(sys.argv[0])
    root = create(olgite.Params(srcdir, sys.argv[1:]))
    bldpath = root.settings()['cantil']['build_path']
    blddir = os.path.join(bldpath, srcdir)
    objs = set()
    blddirs = set()
    cflags = set()
    bldvars = []

    blddirs.add(os.path.join(blddir, 'analysis.d'))
    for i in root.postorder():
        path = os.path.join(bldpath, i.path())

        for obj in i.objs:
            objs.add(os.path.join(path, obj))
            blddirs.add(os.path.dirname(os.path.join(path, obj)))

        for cflag in i.cflags:
            cflags.add(cflag)

    bldvars.append(cncfg.bldvar('cantil_BLDDIR', blddir))
    bldvars.append(cncfg.bldvar('cantil_BLDDIRS', ' '.join(blddirs)))
    bldvars.append(cncfg.bldvar('cantil_OBJS', ' '.join(objs)))
    bldvars.append(cncfg.bldvar('cantil_EXTRA_CFLAGS', ' '.join(cflags)))
    print(' '.join(bldvars))

if __name__ == "__main__":
    main()
