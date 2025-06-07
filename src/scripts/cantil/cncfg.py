from cantil import olgite

class Canode(olgite.Dirnode):
    def __init__(self, path):
        olgite.Dirnode.__init__(self, path)
        self.objs = []
        self.cflags = []

def bldvar(name: str, val: str) -> str:
    return name + '=\"' + val + '\"'
