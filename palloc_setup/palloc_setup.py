import yaml
import os
from os import path


def palloc_setup():
    with open('palloc_setup.yaml') as f:
        data = yaml.load(f, yaml.FullLoader)
        enable_palloc = data['enable_palloc']
        palloc_mask = data['palloc_mask']
        alloc_balance = data['alloc_balance']
        debug_level = data['debug_level']
    
    if enable_palloc == 'yes':
        cmd = 'echo ' + str(palloc_mask) +' > /sys/kernel/debug/palloc/palloc_mask'
        os.system(cmd)
        cmd = 'echo ' + str(debug_level) +' > /sys/kernel/debug/palloc/debug_level'
        os.system(cmd)
        cmd = 'echo ' + str(alloc_balance) +' > /sys/kernel/debug/palloc/alloc_balance'
        os.system(cmd)
        cmd = 'echo 1 > /sys/kernel/debug/palloc/use_palloc'
        os.system(cmd)
    else:
        cmd = 'echo 0 > /sys/kernel/debug/palloc/use_palloc'
        os.system(cmd)

if __name__ == '__main__':
    palloc_setup()