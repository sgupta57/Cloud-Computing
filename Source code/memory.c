#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<sys/types.h>
#include<time.h>
#include<memory.h>
#include<string.h>

#define MEM_SIZE 1374389534.72

int no_of_threads;

double *readWrite(void *in_block_size)
{
        int block_size= *(int *)in_block_size;
        char *src, *dest;
        long int i;
        long int max=(long int)(MEM_SIZE/no_of_threads);
        void *intsrc,*indest;
        src= (char *)malloc(MEM_SIZE);
        memset(src, random(), block_size);
        dest=(char *)malloc(MEM_SIZE);
        intsrc=src;
        indest=dest;
        long int iteration= max/block_size;
        for(i=0;i<iteration;i++)
        {
                memcpy(dest, src, block_size);
                src=src+block_size;
                dest=dest+block_size;
        }
        src=intsrc;
        dest=indest;
        free(src);
        free(dest);
}

void *writeSequential(void *in_block_size)
{
        int block_size= *(int *)in_block_size;
        char *mem;
        int i;
        long int max=(long int)MEM_SIZE/no_of_threads;
        void *intmem;

        mem=(char *)malloc(MEM_SIZE);
        intmem=mem;
        long int iteration = max/block_size;
        for(i=0;i<iteration;i++)
        {
                memset(mem,'1',block_size);
                mem=mem+block_size;
        }
        mem=intmem;
        free(mem);
}

void *writeRandom(void *in_block_size)
{
        int block_size= *(int *)in_block_size;
        char *mem;
        long int i;
        long int rand_temp;
        long int random;
        long int max=(long int)MEM_SIZE/no_of_threads;
        void *intmem;

        mem=(char *)malloc(MEM_SIZE);
        intmem=mem;
        long int iteration = max/block_size;
        for(i=0;i<iteration;i++)
        {
                rand_temp = rand()%max;
                random = mem +(rand_temp) * block_size;
                memset(mem,'1',block_size);
        }
        mem=intmem;
        free(mem);
}

int main(int argc,char *argv[])

{
        int block_sz[4]={8,8192,8388608,83886080};
        int i=0;
        int j=0;
        double total_t=0.0;
        double latency=0.0;
        double throughput=0.0;
        int operation;
        int max;

        struct timespec tstart={0,0} , tend={0,0};

        operation=atoi(argv[1]);
        no_of_threads=atoi(argv[2]);

        pthread_t thread[no_of_threads];


        if(operation==1)
        {
                for(j=0;j<4;j++)
                {
                        clock_gettime(CLOCK_MONOTONIC, &tstart);

                        printf("\nblock size: %d no_of_threads: %d start_clock: %f \n",block_sz[j],no_of_threads,((double)tstart.tv_sec+1.0e-9*tstart.tv_nsec));
                        for(i=0;i<no_of_threads;i++)
                        {
                                pthread_create(&thread[i],NULL,&readWrite,(void *)&block_sz[j]);
                        }
                        for(i=0;i<no_of_threads;i++)
                        {
                                pthread_join(thread[i],NULL);
                        }

                        clock_gettime(CLOCK_MONOTONIC, &tend);
                        printf("block_size: %d no_of_threads: %d end_clock: %f \n",block_sz[j],no_of_threads,((double)tend.tv_sec+1.0e-9*tend.tv_nsec));

                        total_t=((double)tend.tv_sec+1.0e-9*tend.tv_nsec)-((double)tstart.tv_sec+1.0e-9*tstart.tv_nsec);
                        printf("block size: %d no_of_threads: %d total_time: %f \n",block_sz[j],no_of_threads,total_t);

                        throughput = (double) (MEM_SIZE/total_t)/(1024*1024);
                        latency = (double)(total_t/MEM_SIZE)*1e6;

                        if(block_sz[j]==8)
                        {
                        printf("block size: %d no_of_threads: %d Latency: %f\n",block_sz[j],no_of_threads,latency);
                        }
                        else
                        {
                        printf("block size: %d no_of_threads: %d Throughput: %f\n",block_sz[j],no_of_threads,throughput);
                        }


                }
        }
        else if(operation==2)
        {
                for(j=0;j<4;j++)
                {
                        clock_gettime(CLOCK_MONOTONIC, &tstart);
                        printf("\nblock size: %d no_of_threads: %d start_clock: %f \n",block_sz[j],no_of_threads,((double)tstart.tv_sec+1.0e-9*tstart.tv_nsec));

                        for(i=0;i<no_of_threads;i++)
                        {
                                pthread_create(&thread[i],NULL,&writeSequential,(void *)&block_sz[j]);
                        }
                        for(i=0;i<no_of_threads;i++)
                        {
                                pthread_join(thread[i],NULL);
                        }

                        clock_gettime(CLOCK_MONOTONIC, &tend);
                        printf("block size: %d no_of_threads: %d end_clock: %f \n",block_sz[j],no_of_threads,((double)tend.tv_sec+1.0e-9*tend.tv_nsec));

                        total_t=((double)tend.tv_sec+1.0e-9*tend.tv_nsec)-((double)tstart.tv_sec+1.0e-9*tstart.tv_nsec);
                        printf("block size: %d no_of_threads: %d total_time: %f \n",block_sz[j],no_of_threads,total_t);

                        throughput = (double) (MEM_SIZE/total_t)/(1024*1024);

                        latency = (double)(total_t/MEM_SIZE)*1e6;

                        if(block_sz[j]==8)
                        {
                        printf("block size: %d no_of_threads: %d Latency: %f\n",block_sz[j],no_of_threads,latency);
                        }
                        else
                        {
                        printf("block size: %d no_of_threads: %d Throughput: %f\n",block_sz[j],no_of_threads,throughput);
                        }


                }

        }

        else if(operation==3)
        {
                for(j=0;j<4;j++)
                {
                        clock_gettime(CLOCK_MONOTONIC, &tstart);
                        printf("\nblock size: %d no_of_threads: %d start_clock: %f \n",block_sz[j],no_of_threads,((double)tstart.tv_sec+1.0e-9*tstart.tv_nsec));

                        for(i=0;i<no_of_threads;i++)
                        {
                                pthread_create(&thread[i],NULL,&writeRandom,(void *)&block_sz[j]);
                        }
                        for(i=0;i<no_of_threads;i++)
                        {
                                pthread_join(thread[i],NULL);
                        }

                        clock_gettime(CLOCK_MONOTONIC, &tend);
                        printf("block size: %d no_of_threads: %d end_clock: %f \n",block_sz[j],no_of_threads,((double)tend.tv_sec+1.0e-9*tend.tv_nsec));

                        total_t=((double)tend.tv_sec+1.0e-9*tend.tv_nsec)-((double)tstart.tv_sec+1.0e-9*tstart.tv_nsec);
                        printf("block size: %d no_of_threads: %d total_time: %f \n",block_sz[j],no_of_threads,total_t);

                        throughput = (double) (MEM_SIZE/total_t)/(1024*1024);
                        latency = (double)(total_t/MEM_SIZE)*1e6;

                        if(block_sz[j]==8)
                        {
                        printf("block size: %d no_of_threads: %d Latency: %f\n",block_sz[j],no_of_threads,latency);
                        }
                        else
                        {
                        printf("block size: %d no_of_threads: %d Throughput: %f\n",block_sz[j],no_of_threads,throughput);
                        }


                }

        }
return 0;
}
