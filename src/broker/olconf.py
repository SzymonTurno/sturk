import os

def join(olvars):
    settings = olvars.settings()

    olvars.append('cantil_BLDDIRS', olvars.slashify('src', 'broker'))
    olvars.append('cantil_OBJS', olvars.slashify('src', 'broker', 'broker.o'))
