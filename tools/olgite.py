# BSD 3-Clause License
#
# Copyright (c) 2025, Szymon Turno
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its
#    contributors may be used to endorse or promote products derived from
#    this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

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

class Olrule:
    def __init__(self, target: str):
        self.__target = target
        self.__normal = []
        self.__order = []
        self.__steps = []

    def text(self) -> str:
        msg = self.__target + ':'

        for norm in self.__normal:
            msg = msg + ' ' + norm

        if len(self.__order) > 0:
            msg = msg + ' |'

        for order in self.__order:
            msg = msg + ' ' + order

        for step in self.__steps:
            msg = msg + '\n\t' + step
        return msg + '\n'

    def normal_depend(self, prerequisite: str) -> None:
        self.__normal.append(prerequisite)

    def order_depend(self, prerequisite: str) -> None:
        self.__order.append(prerequisite)

    def step(self, step: str) -> None:
        self.__steps.append(step)

class Olvars:
    def __init__(self, root: str, yamls: list):
        self.__root = os.path.normpath(os.path.dirname(root))
        self.__cwd = '.'
        self.__constraints = []
        self.__settings = {}
        self.__variables = {}
        self.__rules = {}
        self.__funcs = ''

        self.__prepare()
        self.__update(yamls)
        self.__join()
        self.__generate()

    def __prepare(self) -> None:
        path = os.path.join(self.acwd(), 'olconf.py')
        spec = importlib.util.spec_from_file_location('olconf', path)
        module = importlib.util.module_from_spec(spec)

        spec.loader.exec_module(module)
        if 'CONSTRAINTS' in module.__dict__.keys():
            self.__constraints = self.__constraints + module.CONSTRAINTS

    def __update(self, yamls: list) -> None:
        yamls.append(os.path.join(self.acwd(), 'olconf.yaml'))
        for yaml_path in yamls:
            if os.path.isfile(yaml_path):
                with open(yaml_path, 'r') as f:
                    combine_into(yaml.safe_load(f), self.__settings)
                    assert_settings(self.__settings, self.__constraints)

    def __join(self) -> None:
        path = os.path.join(self.acwd(), 'olconf.py')
        spec = importlib.util.spec_from_file_location('olconf', path)
        module = importlib.util.module_from_spec(spec)

        spec.loader.exec_module(module)
        module.join(self)

    def __generate(self):
        for key, values in self.__variables.items():
            print(key + ':=' + values[0])
            for value in values[1:]:
                print(key + '+=' + value)

        print('')
        print(self.__funcs)
        for key, rule in self.__rules.items():
            print(rule.text())

    def rule(self, target: str) -> Olrule:
        if target not in self.__rules:
            self.__rules[target] = Olrule(target)
        return self.__rules[target]

    def func(self, name: str, cmd: str) -> None:
        self.__funcs = self.__funcs + name + ' = ' + cmd + '\n'

    def slashify(self, *paths) -> str:
        return os.path.join(*paths).replace('\\', '/')

    def root(self) -> str:
        return os.path.normpath(self.__root)

    def cwd(self) -> str:
        return os.path.normpath(self.__cwd)

    def acwd(self) -> str:
        return os.path.normpath(os.path.join(self.root(), self.cwd()))

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
