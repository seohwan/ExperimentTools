#include <stdio.h>
#include <unistd.h>
#include <sys/times.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>

#define CACHE_MIN (1024*128) /* smallest cache */
#define CACHE_MAX (1024*128) /* largest cache */
//#define CACHE_MIN (1024*1024*8) /* smallest cache */
//#define CACHE_MAX (1024*1024*8) /* largest cache */
#define SAMPLE 1000000 /* to get a larger time sample */
#define TSTEPS 100

int x[CACHE_MAX]; /* array going to randomly access through */

int main() {
    int register i, index;
    int steps, csize, steps_all;
    double sec_temp, sec, WCET ;
	struct timespec  begin, end;
	long res;

	FILE * fpw=fopen("data","w");
	srand((unsigned)time(NULL));

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
