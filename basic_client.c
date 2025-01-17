#include "pipe_networking.h"
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

static void sighandler(int signo){
  if(signo==SIGINT){
    printf("SIGINT caught\n");
    exit(0);
  }
}

int main() {
  signal(SIGINT,sighandler);
  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );

}
