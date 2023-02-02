import yaml
import os
import psutil

def test():
    with open('interference.yaml') as f:
        data = yaml.load(f, yaml.FullLoader)
        workload1_cpuset = data['workload1_cpuset']
        workload1_coloring = data['workload1_coloring']
        workload2_cpuset = data['workload2_cpuset']
        workload2_coloring = data['workload2_coloring']
        workload1_cmd = data['workload1_cmd']
        workload2_cmd = data['workload2_cmd']
        mode = data['mode']
    pid_list = []
    for proc in psutil.process_iter():
        if proc.status() == 'sleeping' and proc.name() == 'sh':
            pid_list.append(proc.pid)
    
    cmd = workload1_cmd + " &"
    os.system(cmd)
    if mode == 'corun':
        cmd = workload2_cmd + " &"
        os.system(cmd)

    for proc in psutil.process_iter():
        if proc.status() == 'running' and proc.name() == workload1_cmd.split('/')[-1]:
            if not os.path.exists("/sys/fs/cgroup/cpuset/core" + str(workload1_cpuset)):
                cmd = "mkdir /sys/fs/cgroup/cpuset/core" + str(workload1_cpuset)
                os.system(cmd)
                cmd = "echo "+ str(workload1_cpuset) + " > /sys/fs/cgroup/cpuset/core" + str(workload1_cpuset) + "/cpuset.cpus"
                os.system(cmd)
            if not os.path.exists("/sys/fs/cgroup/palloc/coloring" + str(workload1_coloring)):
                cmd = "mkdir /sys/fs/cgroup/palloc/coloring" + str(workload1_coloring)
                os.system(cmd)
                cmd = "echo "+ str(workload1_coloring) + " > /sys/fs/cgroup/palloc/coloring" + str(workload1_coloring) + "/palloc.bins"
                os.system(cmd)
            cmd = "echo " + str(proc.pid) + " > /sys/fs/cgroup/cpuset/core" + str(workload1_cpuset) + "/tasks"
            os.system(cmd)
            cmd = "echo " + str(proc.pid) + " > /sys/fs/cgroup/palloc/coloring" + str(workload1_coloring) + "/tasks"
            os.system(cmd)
    for proc in psutil.process_iter():
        if proc.status() == 'sleeping' and proc.name() == 'sh' and proc.pid not in pid_list:
            if not os.path.exists("/sys/fs/cgroup/cpuset/core" + str(workload2_cpuset)):
                cmd = "mkdir /sys/fs/cgroup/cpuset/core" + str(workload2_cpuset)
                os.system(cmd)
                cmd = "echo "+ str(workload2_cpuset) + " > /sys/fs/cgroup/cpuset/core" + str(workload2_cpuset) + "/cpuset.cpus"
                os.system(cmd)
            if not os.path.exists("/sys/fs/cgroup/palloc/coloring" + str(workload2_coloring)):
                cmd = "mkdir /sys/fs/cgroup/palloc/coloring" + str(workload2_coloring)
                os.system(cmd)
                cmd = "echo "+ str(workload2_coloring) + " > /sys/fs/cgroup/palloc/coloring" + str(workload2_coloring) + "/palloc.bins"
                os.system(cmd)
            cmd = "echo " + str(proc.pid) + " > /sys/fs/cgroup/cpuset/core" + str(workload2_cpuset) + "/tasks"
            os.system(cmd)
            cmd = "echo " + str(proc.pid) + " > /sys/fs/cgroup/palloc/coloring" + str(workload2_coloring) + "/tasks"
            os.system(cmd)
if __name__ == '__main__':
    test()
    