import os

def join(olvars):
    settings = olvars.settings()

    olvars.append('sturk_BLDDIRS', os.path.join('src', 'broker'))
    olvars.append('sturk_OBJS', os.path.join('src', 'broker', 'broker.o'))
