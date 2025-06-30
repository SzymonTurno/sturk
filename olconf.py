import os
from sys import platform

def join(olvars):
    olvars.append('CC', 'gcc')
    if platform == 'win32':
        olvars.append('MKDIR', 'mkdir $(subst /,\\,$(1))')
    else:
        olvars.append('MKDIR', 'mkdir -p $(1)')
    olvars.append('cantil_OLCONF', olvars.unix(olvars.path()))
    olvars.append('cantil_INC', '-Iinclude')
    olvars.include('src')
    olvars.include('samples')
    olvars.include('tests')
