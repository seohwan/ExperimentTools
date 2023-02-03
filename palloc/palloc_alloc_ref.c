#include <stdio.h>
#include <unistd.h>
#include <sys/times.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>


//#define CACHE_MIN (1024*512) /* smallest cache */
//#define CACHE_MAX (1024*512) /* largest cache */
#define CACHE_MIN (128) /* smallest cache */
#define CACHE_MAX (1024*1024*2) /* largest cache */
//#define CACHE_MIN (1024*1024*16) /* smallest cache */
//#define CACHE_MAX (1024*1024*512) /* largest cache */
#define SAMPLE 1000000 /* to get a larger time sample */
#define TSTEPS 100

int x[CACHE_MAX]; /* array going to randomly access through */

void palloc_alloc(pid_t mypid, char* cpuset, char* color){
	//printf("PID = %d\n",mypid);
	char* cmd = (char *)malloc(sizeof(char*)*50);
	//printf("%d %s %s \n",mypid, cpuset, color);
	
	if(!(cpuset[0]== '-')){
		//cpuset configuration
		sprintf(cmd, "/sys/fs/cgroup/cpuset/core%s",cpuset);
		if(access(cmd, F_OK)<0){
			sprintf(cmd, "mkdir /sys/fs/cgroup/cpuset/core%s",cpuset);
			system(cmd);
			sprintf(cmd, "echo %s > /sys/fs/cgroup/cpuset/core%s/cpuset.cpus",cpuset, cpuset);
			system(cmd);
		}
		sprintf(cmd, "echo %d > /sys/fs/cgroup/cpuset/core%s/tasks",mypid,cpuset);
		system(cmd);	
	}
	if(!(color[0]== '-')){
		//coloring configuration
		sprintf(cmd, "/sys/fs/cgroup/palloc/coloring%s",color);
		if(access(cmd, F_OK)<0){
			sprintf(cmd, "mkdir /sys/fs/cgroup/palloc/coloring%s",color);
			system(cmd);
			sprintf(cmd, "echo %s > /sys/fs/cgroup/palloc/coloring%s/palloc.bins",color, color);
			system(cmd);
		}
		sprintf(cmd, "echo %d > /sys/fs/cgroup/palloc/coloring%s/tasks",mypid,color);
		system(cmd);
	}
}

int main(int argc, char **argv) {
    int register i, index;
    int steps, csize, steps_all;
    double sec_temp, sec, WCET ;
	struct timespec  begin, end;
	long res;

	FILE * fpw=fopen("data","w");
	srand((unsigned)time(NULL));

	//printf("%s %s", argv[1],argv[2]);
	//printf("PID = %d\n",getpid());
	palloc_alloc(getpid(), argv[1], argv[2]);
	
	//std:system(cmd);
	//std::system("echo "+getpid() + "33");
    for (csize=CACHE_MIN; csize <= CACHE_MAX; csize=csize*2){
		sec = 0.0;
		sec_temp = 0.0;
		steps_all = 0;
		WCET = -1.0;	
		for(steps=0;steps<TSTEPS;steps++){
			clock_gettime(CLOCK_MONOTONIC, &begin);
			for(i=0;i<SAMPLE;i++){
				index = rand() % csize;
				x[index] = x[index] + 1;
			}
			clock_gettime(CLOCK_MONOTONIC, &end);
			res = (end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec);
			if((end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec) < 0.0){
				fprintf(fpw,"Testcase %d read+write: %4.4lf ms\n", steps, sec_temp);
				continue;
			}
			//printf("%4.4lf ",(double)res/1e6);
			
  			clock_gettime(CLOCK_MONOTONIC, &begin);
			for(i=0;i<SAMPLE;i++){
				index = rand() % csize;
			}
			clock_gettime(CLOCK_MONOTONIC, &end);
			if((end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec) < 0.0){
				fprintf(fpw,"Testcase %d read+write: %4.4lf ms\n", steps, sec_temp);
				continue;
			}
			res= res- ((end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec));
			if(res < 0.0){
				fprintf(fpw,"Testcase %d read+write: %4.4lf ms\n", steps, sec_temp);
				continue;
			}
			if (res > WCET){
				WCET = res;
			}
			//printf("%4.4lf %4.4lf\n",(double)((end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec))/1e6, (double)res/1e6);
			sec = sec + res;
			fprintf(fpw,"Testcase %d read+write: %4.4lf ms\n", steps, (double) res/1e6);
			steps_all = steps_all + 1;
		}
		
		printf("Size: %7ld Average read+write: %4.4lf ms WCRT read+write: %4.4lf ms Error ratio %3.2lf%%\n",
	       csize*sizeof(int), (double) sec/1e6/steps_all, (double) WCET/1e6, (double)(TSTEPS- steps_all)/(double)TSTEPS*100);

		fprintf(fpw,"Size: %7ld Average read+write: %4.4lf ms WCRT read+write: %4.4lf ms Error ratio %3.2lf%%\n",
	       csize*sizeof(int), (double) sec/1e6/steps_all, (double) WCET/1e6, (double)(TSTEPS- steps_all)/(double)TSTEPS*100);
	}
	
}
