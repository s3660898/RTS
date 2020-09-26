#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "messages.h"
#include "server.h"
#include "intersection.h"

int main(void) {
  struct server s;
  struct intersection isection;

  /*initialising the server structure*/
  if(server_init(&s)){
    puts("server failed to init!");
    return 1;
  }

  /*initialising internal intersection state*/
  intersection_init(&isection);

  /*closing the IPC server*/
  if(server_destroy(&s)){
    puts("failed to close server!");
    return 1;
  }

  puts("exiting normally!");
  return EXIT_SUCCESS;
}
