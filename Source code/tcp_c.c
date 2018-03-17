#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<time.h>
#include<sys/time.h>
#include<malloc.h>
#include <pthread.h>

#define pack 65536
#define num_iter 131072
#define SERVER_IP "127.0.0.1"
#define PORT 2500

int tcp_socket,num_thrd;
char *pack_srvr;
char *pack_cl;

void *thrd_handler(void *descr)
{
    int i;
    for(i=0;i<num_iter/num_thrd;i++)
    {
    if( send(tcp_socket , pack_srvr , strlen(pack_srvr) , 0) < 0)
        {
            puts("Sending Failed....!!!!\n");
        }
        if( recv(tcp_socket , pack_cl , pack , 0) < 0)
        {
            puts("Receiving Failed.....!!!!\n");
        }
    }

    return 0;
}

int main(int argc , char *argv[])
{
    int i;
    struct sockaddr_in server;
    pack_srvr = (char *) malloc(pack);
    pack_cl = (char *) malloc(pack);
    clock_t start, end;
    double time_taken,latency,throu;
    num_thrd=atoi(argv[1]);
    //scanf("%d", &num_thrd);
    pthread_t thrd[num_thrd];

    tcp_socket = socket(AF_INET , SOCK_STREAM , 0);
    if (tcp_socket == -1)
    {
        printf("Socket not Created....!!!!\n");
      }
      puts("Socket is UP...!!!!\n");

      server.sin_addr.s_addr = inet_addr(SERVER_IP);
      server.sin_family = AF_INET;
      server.sin_port = htons( PORT );

      memset(pack_srvr,'Z',pack);
      if (connect(tcp_socket , (struct sockaddr *)&server , sizeof(server)) < 0)
      {
          perror("No Connection..!!!!\n");
          return 1;
      }

      puts("Connected....!!!!!\n");

      if(1)
      {
          start = clock();
          for(i=0;i<num_thrd;i++)
           {
                  pthread_create(&thrd[i],NULL,thrd_handler,NULL);
           }
          for(i=0;i<num_thrd;i++)
           {
                  pthread_join(thrd[i],NULL);
           }
           end = clock();
           time_taken = ((float) (end - start)) / CLOCKS_PER_SEC;
           latency = ((double)(time_taken * 1000)) / num_iter;
           throu = ((double)(64 * num_iter * 8 *num_thrd))/(time_taken * 1024);
           printf("latency(ms):%f\n",latency);
           printf("throughput(Mb/sec):%f\n",throu);
       }
       close(tcp_socket);
   }
