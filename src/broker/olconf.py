import os

def join(olvars):
    settings = olvars.settings()

    olvars.append('cantil_BLDDIRS', os.path.join('src', 'broker'))
    olvars.append('cantil_OBJS', os.path.join('src', 'broker', 'broker.o'))
