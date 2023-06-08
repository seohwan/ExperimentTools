import yaml
import os
from os import path


def palloc_coloring():
    with open('palloc_coloring.yaml') as f:
        data = yaml.load(f, yaml.FullLoader)
        lxc1_color = data['lxc1_color']
        lxc2_color = data['lxc2_color']
    
    # lxc1
    cmd = "echo "+ str(lxc1_color) + " > /sys/fs/cgroup/palloc/lxc.payload.linux1/palloc.bins"
    os.system(cmd)
    # lxc2, running bash
    cmd = "echo "+ str(lxc2_color) + " > /sys/fs/cgroup/palloc/lxc.payload.linux2/palloc.bins"
    os.system(cmd)
    
if __name__ == '__main__':
    palloc_coloring()