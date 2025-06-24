import os

def join(olvars):
    olvars.append('CC', 'gcc')
    olvars.append('MKDIR', 'mkdir -p')
    olvars.append('cantil_OLCONF', olvars.path())
    olvars.append('cantil_INC', '-I' + os.path.join(olvars.cwd(), 'include'))
    olvars.include('src')
    olvars.include('sample')
    olvars.include('test')
