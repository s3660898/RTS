#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <ncurses.h>

#include "messages.h"
#include "server.h"
#include "intersection.h"
#include "gui.h"
#include "cc_state.h"

int main(void) {
  pthread_t server_thread;
  struct cc_state ccs;

  /*initialising internal intersection state*/
  cc_state_init(&ccs);

  /*initiailising the ncurses gui*/
  gui_init();

  /*initialising the server structure*/
  pthread_create(&server_thread, NULL, server_init, &ccs);

  /*drawing the gui*/
  while(1){
    gui_solve(&ccs);
  }

  /*waiting for the server thread to join*/
  pthread_join(server_thread, NULL);

  /*closing the IPC server*/
#if 0
  if(server_destroy(&s)){
    puts("failed to close server!");
    return 1;
  }
#endif

  puts("exiting normally!");
  return EXIT_SUCCESS;
}
