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

class Olvars:
    def __init__(self, root: str, yamls: list):
        self.__root = os.path.normpath(os.path.dirname(root))
        self.__cwd = os.path.normpath(os.path.dirname(root))
        self.__constraints = []
        self.__settings = {}
        self.__variables = {}

        self.__prepare()
        self.__update(yamls)
        self.__join()
        self.__generate()

    def __prepare(self) -> None:
        path = os.path.join(self.__cwd, 'olconf.py')
        spec = importlib.util.spec_from_file_location('olconf', path)
        module = importlib.util.module_from_spec(spec)

        spec.loader.exec_module(module)
        if 'CONSTRAINTS' in module.__dict__.keys():
            self.__constraints = self.__constraints + module.CONSTRAINTS

    def __update(self, yamls: list) -> None:
        yamls.append(os.path.join(self.__cwd, 'olconf.yaml'))
        for yaml_path in yamls:
            if os.path.isfile(yaml_path):
                with open(yaml_path, 'r') as f:
                    combine_into(yaml.safe_load(f), self.__settings)
                    assert_settings(self.__settings, self.__constraints)

    def __join(self) -> None:
        path = os.path.join(self.__cwd, 'olconf.py')
        spec = importlib.util.spec_from_file_location('olconf', path)
        module = importlib.util.module_from_spec(spec)

        spec.loader.exec_module(module)
        module.join(self)

    def __generate(self):
        for key, values in self.__variables.items():
            print(key + ' = ' + values[0])
            for value in values[1:]:
                print(key + ' += ' + value)

    def slashify(self, *paths) -> str:
        return os.path.join(*paths).replace('\\', '/')

    def root(self) -> str:
        return os.path.normpath(self.__root)

    def cwd(self) -> str:
        return os.path.normpath(self.__cwd)

    def path(self) -> str:
        return os.path.normpath(os.path.join(self.cwd(), 'olconf.py'))

    def settings(self) -> dict:
        return dict(self.__settings)

    def variable(self, key: str) -> list:
        return list(self.__variables[key])

    def include(self, subdir: str) -> None:
        tmp = self.__cwd
        self.__cwd = os.path.join(self.__cwd, subdir)

        self.__prepare()
        self.__update([])
        self.__join()
        self.__cwd = tmp

    def append(self, key: str, val: str) -> None:
        if key not in self.__variables:
            self.__variables[key] = []
        self.__variables[key].append(val)

    def fail(self, e):
        fail(e)

if __name__ == "__main__":
    Olvars(sys.argv[1], sys.argv[2:])
