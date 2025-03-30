import os
import yaml
import argparse

def get_args():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "config", nargs="+", help="pass yaml configuration files")
    parser.add_argument(
        "-f", "--Filter", help="pass filter for desired library", required=True)
    return parser.parse_args()    

def parse_yaml(path, module):
    with open(path, 'r') as f:
        return yaml.safe_load(f)["ub-" + module]

def get_cfg(args):
    cfg = {}
    for i in reversed(args.config):
        cfg = cfg | parse_yaml(i, args.Filter)
    return cfg

def print_cfg(cfg, lib):
    dirname = os.path.dirname(__file__)
    for key, val in cfg.items():
        print(dirname + "/" + lib + "/" + val + "/" + key + ".o")

def main():
    args = get_args()
    cfg = get_cfg(args)
    print_cfg(cfg, args.Filter)

if __name__ == "__main__":
    main()
