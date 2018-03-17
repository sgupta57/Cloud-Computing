#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>
#include<malloc.h>

#define pack 65536
#define PORT 2500

void *thrd_handler(void *server_s)
{

  int tcp_sock = *(int*)server_s;
  int rcv;
  char *pack_cl=(char *) malloc(pack);

  while( (rcv = recv(tcp_sock , pack_cl , pack , 0)) > 0 )
  {

      write(tcp_sock , pack_cl , strlen(pack_cl));
  }

  if(rcv == 0)
  {
      puts("Client Disconnected..!!!!\n");
      fflush(stdout);
  }
  else if(rcv == -1)
  {
      perror("Receiving Failed....!!!!!\n");
    }

    free(server_s);

    return 0;
}

int main(int argc , char *argv[])
{
    int server_s , client_s , sock_size , *n_sock, rcv;
    struct sockaddr_in server , client;
    char *pack_cl=(char *) malloc(pack);

    server_s = socket(AF_INET , SOCK_STREAM , 0);
    if (server_s == -1)
    {
        printf("Socket Down..!!!!!\n");
    }
    puts("Socket created...!!!!\n");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( PORT );

    if( bind(server_s,(struct sockaddr *)&server , sizeof(server)) == -1)
    {

              perror("Bind Failed");
              return 1;
          }
          puts("Bind Done");

          listen(server_s , 3);

          puts("Waiting for Client Connections");
          sock_size = sizeof(struct sockaddr_in);

          while( (client_s = accept(server_s, (struct sockaddr *)&client, (socklen_t*)&sock_size)) )
          {
              puts("Connection Accepted");

              pthread_t n_thrd;
              n_sock = malloc(1);
              *n_sock = client_s;

              if( pthread_create( &n_thrd , NULL ,  thrd_handler , (void*) n_sock) < 0)
              {
                  perror("Couldn't create Thread..!!!\n");
                  return 1;
              }

              puts("Connected to new client\n");
          }

          if (client_s < 0)
          {
              perror("Could'nt Accept..!!!!\n");
              return 1;
  }

  return 0;
}
