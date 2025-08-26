import os
from sys import platform

def add_all(olvars):
    rule = olvars.rule('.PHONY')

    rule.normal_depend('all')

    rule = olvars.rule('all')
    rule.normal_depend(os.path.join('$(cantil_BLDDIR)', 'libcantil.a'))

def join(olvars):
    olvars.append('CC', 'gcc')
    olvars.append('cantil_INC', '-I' + os.path.join(olvars.acwd(), 'include'))
    olvars.append('tools_DIR', os.path.join(olvars.acwd(), 'tools'))
    olvars.func('MKDIR', 'mkdir -p $@')
    add_all(olvars)
    olvars.include('src')
    olvars.include('samples')
    olvars.include('tests')
