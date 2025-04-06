import os
import yaml
import argparse

CONSTRAINTS = [
    (["common", "cver", "iso"], ["osal", "mutex", "posix"])
]

def get_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("config", nargs="+",
        help="pass yaml configuration files")
    parser.add_argument("-s", "--Section", help="pass section filter")
    parser.add_argument("-p", "--Parameter", help="pass parameter filter")
    return parser.parse_args()

def parse_yaml(path):
    with open(path, 'r') as f:
        return yaml.safe_load(f)

def get_cfg(configs):
    cfg = {}
    for i in reversed(configs):
        yaml = parse_yaml(i)
        for key in yaml:
            if key in cfg:
                cfg[key] = cfg[key] | yaml[key]
            else:
                cfg[key] = yaml[key]
    return cfg

def print_objs(cfg):
    for key, val in cfg.items():
        print(val + "/" + key + ".o")

def find_cfg(cfg, entry):
    section = "ub-" + entry[0]
    if section in cfg:
        if entry[1] in cfg[section]:
            return cfg[section][entry[1]] == entry[2]
    return False

def check_cfg(cfg):
    e = ""
    for constraint in CONSTRAINTS:
        if find_cfg(cfg, constraint[0]) and find_cfg(cfg, constraint[1]):
            e = e + "\n" + str(constraint)
    if e != "":
        tmp = "\nUB configuration not supported, "
        tmp += "violated following constraints:"
        e = tmp + e + ".\n"
        raise SystemExit(e)

def main():
    args = get_args()
    cfg = get_cfg(args.config)
    check_cfg(cfg)
    if args.Section is None:
        print(cfg)
    elif args.Parameter is not None:
        print(cfg["ub-" + args.Section][args.Parameter])
    else:
        print_objs(cfg["ub-" + args.Section])

if __name__ == "__main__":
    main()
