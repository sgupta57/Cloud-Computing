#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<time.h>
#include<sys/time.h>
#include<malloc.h>
#include <pthread.h>

#define size 65000
#define iter 131072

char *pack_srvr;
char *pack_cl;
int src_socket, add_size,num_thrd;
struct sockaddr_in udp_serv;

void *connection_handler(void *socket_desc)
{
  int i,send,rcv;
  for(i=0;i<iter/num_thrd;i++)
  {
  send = sendto(src_socket, pack_srvr, strlen(pack_srvr) , 0 , (struct sockaddr *) &udp_serv, add_size);
  if (send < 0)
      {
        puts("Send Failed");
          exit(1);
      }
      rcv = recvfrom(src_socket, pack_cl, size, 0, (struct sockaddr *) &udp_serv, &add_size);
      if ( rcv < 0)
      {
          puts("Receive Failed");
          exit(1);
        }
  }
  return 0;
}

int main(int argc,char *argv[])
{

    int i;
    add_size = sizeof(udp_serv);
    pack_srvr = (char *) malloc(size);
    pack_cl = (char *) malloc(size);
    clock_t start, end;
    float time_taken;
    num_thrd=atoi(argv[1]);
    //scanf("%d", &num_thrd);
    pthread_t thrd[num_thrd];
    double band,latency;

    src_socket=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if ( src_socket < 0)
    {
        printf("Socket Creation Failed");
        exit(1);
    }

    memset((char *) &udp_serv, 0, sizeof(udp_serv));
    udp_serv.sin_family = AF_INET;
    udp_serv.sin_port = htons( 3500 );
    memset(pack_srvr,'$',size);

    if (inet_aton("127.0.0.1" , &udp_serv.sin_addr) == 0)
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

    if(1)
    {
        start = clock();
        for(i=0;i<num_thrd;i++)
        {
              pthread_create(&thrd[i],NULL,connection_handler,NULL);
       }
      for(i=0;i<num_thrd;i++)
       {
              pthread_join(thrd[i],NULL);
       }
      end = clock();
      time_taken = ((float) (end - start)) / CLOCKS_PER_SEC;
      latency = ((double)(time_taken * 1000)) / iter;
      band = ((double)(64 * iter * 8 *num_thrd))/(time_taken * 1024);
      printf("latency in milliseconds:%f\n",latency);
      printf("throughput in Mb/sec:%f\n",band);

  }

  close(src_socket);
  return 0;
}
