from cantil import breeck

class Canode(breeck.Dirnode):
    def __init__(self, path):
        breeck.Dirnode.__init__(self, path)
        self.objs = []
        self.cflags = []

def bldvar(name: str, val: str) -> str:
    return name + '=\"' + val + '\"'
