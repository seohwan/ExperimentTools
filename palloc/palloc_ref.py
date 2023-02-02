import yaml
import os
import psutil

def test():
    with open('palloc_ref.yaml') as f:
        data = yaml.load(f, yaml.FullLoader)
        cpuset = data['cpuset']
        task_coloring = data['task_coloring']
        workload = data['workload']
    
    for proc in psutil.process_iter():
        if proc.status() == 'running' and proc.name() == workload:
            #print(proc.name(), proc.status(), proc.pid)
            if not os.path.exists("/sys/fs/cgroup/cpuset/core" + str(cpuset)):
                cmd = "mkdir /sys/fs/cgroup/cpuset/core" + str(cpuset)
                os.system(cmd)
                cmd = "echo "+ str(cpuset) + " > /sys/fs/cgroup/cpuset/core" + str(cpuset) + "/cpuset.cpus"
                os.system(cmd)
            cmd = "echo " + str(proc.pid) + " > /sys/fs/cgroup/cpuset/core" + str(cpuset) + "/tasks"
            os.system(cmd)

            if not os.path.exists("/sys/fs/cgroup/palloc/coloring" + str(task_coloring)):
                cmd = "mkdir /sys/fs/cgroup/palloc/coloring" + str(task_coloring)
                os.system(cmd)
                cmd = "echo "+ str(task_coloring) + " > /sys/fs/cgroup/palloc/coloring" + str(task_coloring) + "/palloc.bins"
                os.system(cmd)
            cmd = "echo " + str(proc.pid) + " > /sys/fs/cgroup/palloc/coloring" + str(task_coloring) + "/tasks"
            os.system(cmd)

if __name__ == '__main__':
    test()
    