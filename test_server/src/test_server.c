#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "server.h"

int main(void) {
  pthread_t t_server;

  pthread_create(&t_server, NULL, server_init, NULL);
  puts("server spawned");
  pthread_join(t_server, NULL);

  puts("exiting");
  return 0;
}
