#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>

#define MX_ITR 5368709120.00

int no_of_threads;

void *readWrite(void *in_block_size)
{

        int block_size = *(int *)in_block_size;

        //printf("\n  Block size :: %d", block_size);
        FILE *fh;
        char *buffer;
        int i,j;
        buffer=(char *)malloc(block_size);
        int max= (MX_ITR/ block_size) /no_of_threads;

        fh=fopen("file.txt","r+");

        for(j=0;j<max;j++)
        {
                fread(buffer,block_size,1,fh);
        }
        fflush(fh);

        FILE *fh1;
        fh1=fopen("file1.txt","w+");
        for(j=0;j<max;j++)
        {
                fwrite(buffer,1,block_size,fh1);
        }
        fclose(fh1);
}

void *seqRead(block_size)
{
        FILE *fh;
        int i=0;

        int max= MX_ITR/block_size/no_of_threads;

        char *buffer;
        buffer=(char *)malloc(block_size);

        fh= fopen("file.txt","r+");

        for(i=0;i<max;i++)
        {
                fread(buffer,block_size,1,fh);
        }
        fclose(fh);
}

void *randomRead(block_size)
{
        FILE *fh;
        int i=0;
        char *buffer;
        buffer=(char *)malloc(block_size);

        int max= MX_ITR/block_size/no_of_threads;

        int random;
        fh= fopen("file.txt","r+");
        for(i=0;i<max;i++)
        {
                random=rand()%block_size;
                fseek(fh,random,SEEK_SET);
                fread(buffer,block_size,1,fh);
        }
        fclose(fh);
}

int main(int argc,char *argv[])
{
        int blk_size[4]={8,8192,8388608,83886080};
        double total_t=0.0, latency, throughput;
        int i=0,j=0;
        int operation;
        int max;
        clock_t start_clock, end_clock,start_clock1, end_clock1, start_clock2, end_clock2;

        operation=atoi(argv[1]);
        no_of_threads=atoi(argv[2]);
        //printf("op %d, thread %d",operation, no_of_threads);

        pthread_t thread[no_of_threads];

        if(operation==1)
        {

        for(j=0;j<4;j++)        //iterate through the block size
        {
                start_clock=clock();
                //printf("start_clock: %ld\n",start_clock/CLOCKS_PER_SEC);

                for(i=0;i<no_of_threads;i++)
                {
                        pthread_create(&thread[i],NULL,&readWrite,(void *)&blk_size[j]);
                }

                for(i=0;i<no_of_threads;i++)
                {
                        pthread_join(thread[i],NULL);
                }


                end_clock=clock();
                //printf("end_clock: %ld\n",end_clock);

                total_t=(double)(end_clock-start_clock)/CLOCKS_PER_SEC;
                //printf("Total Time: %f\n",total_t);
                throughput = (double) (MX_ITR/total_t)/(1024*1024);
                latency = (double)(total_t/MX_ITR)*1e3;
                if(blk_size[j]==8)
                {
                        printf("block size: %d Latency: %f\n",blk_size[j],latency);
                }
                else
                {
                        printf("block size: %d Throughput: %f\n",blk_size[j],throughput);
                }
        }

        }
        else if(operation==2)
        {

        for(j=0;j<4;j++)
        {
                start_clock1=clock();
                //printf("start_clock: %ld\n",start_clock1);

                for(i=0;i<no_of_threads;i++)
                {
                        pthread_create(&thread[i],NULL,&seqRead,blk_size[j]);
                }

                for(i=0;i<no_of_threads;i++)
                {
                        pthread_join(thread[i],NULL);
                }


                end_clock1=clock();
                //printf("end_clock: %ld\n",end_clock1);

                total_t=(double)(end_clock1-start_clock1)/CLOCKS_PER_SEC;
                //printf("Total Time: %f\n",total_t);

                throughput = (double) (MX_ITR/total_t)/(1024*1024);
                latency = (double)(total_t/MX_ITR)*1e3;
                if(blk_size[j]==8)
                {
                        printf("block size: %d Latency: %f\n",blk_size[j],latency);
                }
                else
                {
                        printf("block size: %d Throughput: %f\n",blk_size[j],throughput);
                }
        }
        }
        else if(operation==3)
        {

        for(j=0;j<4;j++)
        {

                start_clock2=clock();
                //printf("start_clock: %ld\n",start_clock1);
                for(i=0;i<no_of_threads;i++)
                {
                        pthread_create(&thread[i],NULL,&randomRead,blk_size[j]);
                }

                for(i=0;i<no_of_threads;i++)
                {
                        pthread_join(thread[i],NULL);
                }


                end_clock2=clock();
                //printf("end_clock: %ld\n",end_clock2);

                total_t=(double)(end_clock2-start_clock2)/CLOCKS_PER_SEC;
                //printf("Total Time: %f\n",total_t);

                throughput = (double) (MX_ITR/total_t)/(1024*1024);
                latency = (double)(total_t/MX_ITR)*1e3;

                if(blk_size[j]==8)
                        {
                                printf("block size: %d Latency: %f\n",blk_size[j],latency);
                        }
                else
                        {
                                printf("block size: %d Throughput: %f\n",blk_size[j],throughput);
                        }
        }
        }
        else exit(0);
        return 0;
}
