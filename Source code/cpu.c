#include<stdio.h>
#include<pthread.h>
#include<sys/types.h>
#include<immintrin.h>
# define MAX_ITR 1000000000

int no_of_threads;

void *cpu_benchmark_flops()
{
        __m256 ymm2,ymm3,ymm4,ymm5;

        int i=0;
        int max= MAX_ITR/no_of_threads;

        __m256 ymm0=_mm256_set_ps(1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8);
        __m256 ymm1=_mm256_set_ps(4.1,5.2,6.3,7.4,8.5,9.6,10.7,11.8);

        for(i=0;i<max;i++)
        {
                ymm2= _mm256_add_ps(ymm0,ymm1);
        }
}


void *cpu_benchmark_iops()
{
        int i=0;
        int max= MAX_ITR/no_of_threads;

        __m256i ymm6=_mm256_setr_epi32(1,2,3,5,6,7,8,9);
        __m256i ymm7=_mm256_setr_epi32(4,5,6,7,8,9,10,11);
        __m256i ymm8;

        for(i=0;i<max;i++)
        {
                ymm8=_mm256_add_epi32(ymm6,ymm7);
        }
}


int main(int argc,char *argv[])
{
        float flops;
        float iops;
        float total_t=0.0;
        int i=0;
        int operation;
        clock_t start_clock, end_clock;

        operation=atoi(argv[1]);
        no_of_threads=atoi(argv[2]);

        pthread_t thread[no_of_threads];

        if(operation==1)
        {
                start_clock=clock();

                for(i=0;i<no_of_threads;i++)
                {
                        pthread_create(&thread[i],NULL,&cpu_benchmark_flops,NULL);
                }

                for(i=0;i<no_of_threads;i++)
                {
                        pthread_join(thread[i],NULL);
                }

                end_clock=clock();

                total_t=(double)(end_clock-start_clock)/CLOCKS_PER_SEC;
                //printf("Total Time: %f\n",total_t);

                flops=(1000000000.00*8*no_of_threads)/(long double)total_t;

                printf("Thread: %d\tGFLOPS: %f\n",i,(double)(flops/1000000000));
        }
        else if(operation==2)
        {
                start_clock=clock();

                for(i=0;i<no_of_threads;i++)
                {
                        pthread_create(&thread[i],NULL,&cpu_benchmark_iops,NULL);
                }

                for(i=0;i<no_of_threads;i++)
                {
                        pthread_join(thread[i],NULL);
                }

                end_clock=clock();

                total_t=(double)(end_clock-start_clock)/CLOCKS_PER_SEC;
                //printf("Total Time: %f\n",total_t);

                iops=(1000000000.00*8*no_of_threads)/total_t;

                printf("Thread: %d\tGIOPS: %f\n",i,(double)(iops/1000000000));
        }
        else exit(0);
        return(0);
}
