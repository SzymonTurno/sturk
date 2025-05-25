import os
import sys

sys.path.append(os.path.join(os.path.dirname(sys.argv[0]), 'scripts'))

from cydiom import breeck
from cydiom import cycfg

def create(params):
    node = cycfg.Cynode(params)
    settings = node.settings()['cydiom']

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
        ['cydiom', 'cver', 'iso'],
        ['cydiom', 'osal', 'mutex', 'posix']
    )
    node.include('algo')
    node.include('broker')
    node.include('logger')
    node.include('osal')
    return node

def main():
    srcdir = os.path.dirname(sys.argv[0])
    root = create(breeck.Params(srcdir, sys.argv[1:]))
    bldpath = root.settings()['cydiom']['build_path']
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

    bldvars.append(cycfg.bldvar('cy-blddir', blddir))
    bldvars.append(cycfg.bldvar('cy-blddirs', ' '.join(blddirs)))
    bldvars.append(cycfg.bldvar('cy-objs', ' '.join(objs)))
    bldvars.append(cycfg.bldvar('cy-extra-cflags', ' '.join(cflags)))
    print(' '.join(bldvars))

if __name__ == "__main__":
    main()
