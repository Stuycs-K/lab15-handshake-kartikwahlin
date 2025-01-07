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
    unlink(WKP);
    exit(0);
  }
}

int main() {
  signal(SIGINT,sighandler);
  signal(SIGPIPE, SIG_IGN);
  int to_client;
  int from_client;
  while(1){
    from_client = server_handshake( &to_client );
  }

}
