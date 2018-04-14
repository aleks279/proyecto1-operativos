#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include "my_pthreads.h" //for threading , link with lpthread
 
//the thread function
void *connection_handler(void *);

char *priority, *resources, *port;
int num_threads;
int received_clients = 0;

int main(int argc , char *argv[])
{
  int socket_desc , client_sock , c , *new_sock;
  struct sockaddr_in server , client;

  int opt;
  while ((opt = getopt(argc, argv, "n:P:r:p:")) != -1)
    switch (opt) {
      case 'n':
        num_threads = atoi(optarg);
        break;
      case 'P':
        priority = strdup(optarg);
        break;
      case 'r':
        resources = strdup(optarg);
        break;
      case 'p':
        port = strdup(optarg);
        break;
      default:
        printf("You chose poorly\n"); exit(1);
    }

  if (num_threads == NULL || priority == NULL || resources == NULL || port == NULL) {
    printf("USAGE: -n <number of threads> -P <priority> -r <reources> -p <port number>\n");
    exit(1);
  }
     
  //Create socket
  socket_desc = socket(AF_INET , SOCK_STREAM , 0);
  if (socket_desc == -1)
  {
    printf("Could not create socket");
  }
  puts("Socket created");
   
  //Prepare the sockaddr_in structure
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons( 8888 );
   
  //Bind
  if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
  {
    //print the error message
    perror("bind failed. Error");
    return 1;
  }
  puts("bind done");
   
  //Listen
  listen(socket_desc , 3);
   
  //Accept and incoming connection
  puts("Waiting for incoming connections...");
  c = sizeof(struct sockaddr_in);

  while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
  {
    if (received_clients < num_threads) {
      puts("Connection accepted");
       
      struct thread* sniffer_thread;
      new_sock = malloc(1);
      *new_sock = client_sock;
       
      if( my_thread_create_with_param(&sniffer_thread, connection_handler, (void*)new_sock) < 0)
      {
        perror("could not create thread");
        return 1;
      }
       
      //Now join the thread , so that we dont terminate before the thread
      //pthread_join( sniffer_thread , NULL);
      puts("Handler assigned");
      received_clients++;
    } else {
      perror("Max connections reached");
      return 1;
    }
  }
   
  if (client_sock < 0)
  {
    perror("accept failed");
    return 1;
  }
   
  return 0;
}
 
/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
  //Get the socket descriptor
  int sock = *(int*)socket_desc;
  char *message , *client_message;

  int step = 0;
  int sleeper;
  char *burguer_process[5] = {
      "1. Bread",
      "2. Steak",
      "3. Lettuce",
      "4. Put all together",
      "5. Deliver"
  };

  if(*priority == 'high') {
    sleeper = 1;
  } else if (*priority == 'medium') {
    sleeper = 3;
  } else {
    sleeper = 5;
  }
     
  //Receive a message from client
  while( step < 5 )
  {
    client_message = burguer_process[step];
    //Send the message back to client
    write(sock , client_message , strlen(client_message));
    write(sock , "\n" , strlen("\n"));
    step++;
    sleep(sleeper);
  }

  puts("Client disconnected");
  fflush(stdout);

  //Free the socket pointer
  free(socket_desc);
  received_clients--;
   
  return 0;
}
