import os
import sys
import yaml
import importlib.util

def __find_entry(settings, entry) -> bool:
    if not settings or not entry:
        return True
    if not isinstance(settings, dict) and settings == entry[0]:
        return True
    if entry[0] in settings:
        return __find_entry(settings[entry[0]], entry[1:])
    return False

def __eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

def fail(e):
    __eprint(e)
    sys.exit(1)

def assert_settings(settings: dict, constraints: list) -> None:
    hdr = '\nOlgite configuration not supported, '
    hdr += 'violated following constraints:'
    e = ''

    for constraint in constraints:
        found0 = __find_entry(settings, constraint[0])
        found1 = __find_entry(settings, constraint[1])
        if found0 and found1:
            if constraint != constraints[0]:
                e = e + ','
            e = e + '\n' + str(constraint)

    if e != '':
        fail(hdr + e + '.\n')

def combine_into(d: dict, combined: dict) -> None:
    for k, v in d.items():
        if isinstance(v, dict):
            combine_into(v, combined.setdefault(k, {}))
        elif k not in combined:
            combined[k] = v

class Params:
    def __init__(self, path: str, yamls: list):
        self.path = path
        self.settings = {}
        self.constraints = []

        self.update(yamls)

    def update(self, yamls: list):
        yamls.append(os.path.join(self.path, 'olconf.yaml'))
        for yaml_path in yamls:
            if os.path.isfile(yaml_path):
                with open(yaml_path, 'r') as f:
                    combine_into(yaml.safe_load(f), self.settings)
                    assert_settings(self.settings, self.constraints)

class Dirnode:
    def __init__(self, params: Params):
        self.__path = params.path
        self.__settings = params.settings
        self.__constraints = params.constraints
        self.__children = {}

    def path(self) -> str:
        return str(self.__path)

    def settings(self) -> dict:
        return dict(self.__settings)

    def include(self, subdir: str) -> None:
        path = os.path.join(self.__path, subdir)
        olgite_path = os.path.join(path, 'olconf.py')
        spec = importlib.util.spec_from_file_location('olconf', olgite_path)
        module = importlib.util.module_from_spec(spec)
        params = Params(path, [])

        params.settings = self.__settings
        params.constraints = self.__constraints
        params.update([])
        spec.loader.exec_module(module)
        self.__children[subdir] = module.create(params)

    def append_constraint(self, entry0: list, entry1: list) -> None:
        self.__constraints.append((entry0, entry1))

    def postorder(self):
        for child in self.__children.values():
            yield from child.postorder()
        yield self

    def fail(self, e):
        fail(e)
