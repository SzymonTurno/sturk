import os
from sys import platform

def join(olvars):
    olvars.append('CC', 'gcc')
    olvars.append('cantil_INC', '-I' + olvars.slashify(olvars.acwd(), 'include'))
    olvars.append('tools_DIR', olvars.slashify(olvars.acwd(), 'tools'))
    olvars.include('src')
    olvars.include('samples')
    olvars.include('tests')
