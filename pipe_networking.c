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
  int from_client = server_setup();
  int piddler;
  read(from_client,&piddler,sizeof(piddler));
  int fifofd = open(&piddler,O_WRONLY);
  int synack = piddler+1;
  *to_client = fifofd;
  write(fifofd, &synack,sizeof(synack)); // synack is the return value(PID + 1)
  read(from_client,&piddler,sizeof(piddler)); //Uses same "PIDDLER" var for all reads
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
  int from_server;
  int * griddy;
  *griddy = getpid();
  mkfifo(griddy,0666);
  int fifofd = open(WKP, O_WRONLY); //FIFOFD IS WKP HERE
  write(fifofd, griddy,sizeof(griddy));
  int wrfd = open(griddy,O_RDONLY);
  unlink(griddy);
  return from_server;
  int synack;
  read(*griddy,&synack,sizeof(synack));
  int ack = synack + 1;
  write(fifofd, &ack,sizeof(ack));
}


/*=========================
  server_connect
  args: int from_client

  handles the subserver portion of the 3 way handshake

  returns the file descriptor for the downstream pipe.
  =========================*/
int server_connect(int from_client) {
  int to_client  = 0;
  return to_client;
}
