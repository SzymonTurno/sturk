import os
import yaml
import argparse

def get_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("config", nargs="+",
        help="pass yaml configuration files")
    parser.add_argument("-s", "--Section", help="pass section filter",
        required=True)
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

def main():
    args = get_args()
    cfg = get_cfg(args.config)
    if (args.Parameter is not None):
        print(cfg["ub-" + args.Section][args.Parameter])
    else:
        print_objs(cfg["ub-" + args.Section])

if __name__ == "__main__":
    main()
