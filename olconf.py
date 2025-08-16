import os
from sys import platform

def add_all(olvars):
    rule = olvars.rule('.PHONY')

    rule.normal_depend('all')

    rule = olvars.rule('all')
    rule.normal_depend(olvars.slashify('$(cantil_BLDDIR)', 'libcantil.a'))

def join(olvars):
    olvars.append('CC', 'gcc')
    olvars.append('cantil_INC', '-I' + olvars.slashify(olvars.acwd(), 'include'))
    olvars.append('tools_DIR', olvars.slashify(olvars.acwd(), 'tools'))
    if platform == 'win32':
        olvars.func('MKDIR', 'mkdir $(subst /,\\,$(1))')
    else:
        olvars.func('MKDIR', 'mkdir -p $(1)')
    add_all(olvars)
    olvars.include('src')
    olvars.include('samples')
    olvars.include('tests')
