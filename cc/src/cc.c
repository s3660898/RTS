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
  struct server s;
  struct cc_state ccs;

  /*initialising the server structure*/
  if(server_init(&s)){
    puts("server failed to init!");
    return 1;
  }

  /*initialising internal intersection state*/
  cc_state_init(&ccs);

  /*initiailising the ncurses gui*/
  gui_init();

  /*drawing the gui*/
  gui_solve(&ccs);
  getch();

  /*closing the IPC server*/
  if(server_destroy(&s)){
    puts("failed to close server!");
    return 1;
  }

  puts("exiting normally!");
  return EXIT_SUCCESS;
}
