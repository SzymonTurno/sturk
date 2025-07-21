import os
from sys import platform

def join(olvars):
    olvars.append('CC', 'gcc')
    olvars.append('cantil_OLCONF', olvars.slashify(olvars.path()))
    olvars.append('cantil_INC', '-I' + olvars.slashify(olvars.cwd(), 'include'))
    olvars.include('src')
    olvars.include('samples')
    olvars.include('tests')
