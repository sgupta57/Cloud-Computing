#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define pack 65000


int main(void)
{
    struct sockaddr_in client, server;

    int udp_sock, i,recv,sr_bind,s_send;
    int add_size = sizeof(server);
    char *pack_cl=(char *) malloc(pack);

    udp_sock=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udp_sock < 0)
    {
        printf("Socket Creation Failed");
        exit(1);
    }

    memset((char *) &client, 0, sizeof(client));

    client.sin_family = AF_INET;
    client.sin_port = htons(3500);
    client.sin_addr.s_addr = htonl(INADDR_ANY);

        sr_bind = bind(udp_sock , (struct sockaddr*)&client, sizeof(client) );
        if( sr_bind < 0)
        {
          printf("Binding Failed");
          exit(1);
        }

        while(1)
        {
            printf("Waiting for Connection\n");
            fflush(stdout);

            recv = recvfrom(udp_sock, pack_cl, pack, 0, (struct sockaddr *) &server, &add_size);
            if (recv < 0)
            {
                puts("Receive Failed");
                exit(1);
            }
            s_send = sendto(udp_sock, pack_cl, strlen(pack_cl), 0, (struct sockaddr*) &server, add_size);
               if ( s_send < 0)
               {
                     puts("Send Failed");
                   exit(1);
               }
         }

           close(udp_sock);
           return 0;
       }
