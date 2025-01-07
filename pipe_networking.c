#include "pipe_networking.h"
//UPSTREAM = to the server / from the client
//DOWNSTREAM = to the client / from the server
/*=========================
  server_setup

  creates the WKP and opens it, waiting for a  connection.
  removes the WKP once a connection has been made

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_setup() {
  int from_client = 0;
  mkfifo(WKP,0666);
  int fifofd = open(WKP, O_RDONLY);
  from_client = fifofd;
  unlink(WKP);
  return from_client;
}

/*=========================
  server_handshake
  args: int * to_client

  Performs the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe (Client's private pipe).

  returns the file descriptor for the upstream pipe (see server setup).
  =========================*/
int server_handshake(int *to_client) {
  srand(time(NULL));
  printf("Server start\n");
  int from_client = server_setup();
  int piddler;
  char fiddler[12];
  read(from_client,&piddler,sizeof(piddler));//NEEDS TO USE FIDDLER FOR THE READ TARGET MY GUYS
  printf("Server reads %d via WKP\n",piddler);
  sprintf(fiddler, "%d",piddler);
  int fifofd = open(fiddler,O_WRONLY);
  int synack = piddler+1;
  *to_client = fifofd;
  write(fifofd, &synack,sizeof(synack)); // synack is the return value(PID + 1)
  printf("server wrote %d via PP\n",synack);
  read(from_client,&piddler,sizeof(piddler)); //Uses same "PIDDLER" var for all reads
  printf("server read %d\n",piddler);
  int x = rand() % 100;
  while(write(fifofd, &x,sizeof(x))!=-1){
    printf("Server wrote %d\n",x);
    x = rand() & 100;
    sleep(1);
  }
  printf("loop over\n");
  close(from_client);
  printf("fromclient closed\n" );
  close(*to_client);
  printf("connection closed\n");
  return from_client;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  printf("Client start\n");
  int from_server;
  int griddy;
  griddy = getpid();
  printf("PID gotten\n");
  char fiddy[12];
  sprintf(fiddy, "%d",griddy);
  printf("pid converted\n");
  mkfifo(fiddy,0666);
  printf("PP created\n");
  int fifofd = open(WKP, O_WRONLY); //FIFOFD IS WKP HERE
  write(fifofd, &griddy,sizeof(griddy));
  printf("Client wrote %d via WKP\n",griddy);//WIP - SEND STRING PID AN JUST USE THAT BRO
  int wrfd = open(fiddy,O_RDONLY);//OPEN THE PP
  unlink(fiddy);
  int synack;
  read(wrfd,&synack,sizeof(synack));
  printf("client read %d via PP\n",synack);
  int ack = synack + 1;
  write(fifofd, &ack,sizeof(ack));
  printf("client wrote %d\n",ack);
  int x;
  while(read(wrfd, &x,sizeof(x))>0){
    printf("client read: %d\n",x);
    sleep(1);
  }
  return from_server;
}


/*=========================
  server_connect
  args: int from_client

  handles the subserver portion of the 3 way handshake

  returns the file descriptor for the downstream pipe.
  =========================*/
int server_connect(int from_client) {
  printf("fork starting up\n");
  int to_client  = 0;
  srand(time(NULL));
  int piddler;
  char fiddler[12];
  read(from_client,&piddler,sizeof(piddler));//NEEDS TO USE FIDDLER FOR THE READ TARGET MY GUYS
  printf("Server reads %d via WKP\n",piddler);
  sprintf(fiddler, "%d",piddler);
  int fifofd = open(fiddler,O_WRONLY);
  int synack = piddler+1;
  to_client = fifofd;
  write(fifofd, &synack,sizeof(synack)); // synack is the return value(PID + 1)
  printf("server wrote %d via PP\n",synack);
  read(from_client,&piddler,sizeof(piddler)); //Uses same "PIDDLER" var for all reads
  printf("server read %d\n",piddler);
  int x = rand() % 100;
  while(write(fifofd, &x,sizeof(x))!=-1){
    printf("Server wrote %d\n",x);
    x = rand() & 100;
    sleep(1);
  }
  printf("loop over\n");
  close(from_client);
  printf("fromclient closed\n" );
  close(to_client);
  printf("connection closed\n");
  return to_client;
}
