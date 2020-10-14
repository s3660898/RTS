#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <ncurses.h>

#include "messages.h"
#include "server.h"
#include "intersection.h"
#include "gui.h"
#include "cc_state.h"
#include "clients.h"

void signal_handler(int signo){
  if(signo == SIGINT){

    /*ncurses cleanup*/
    endwin();

    puts("exiting via sigint!");
    exit(0);
  }
}

int main(void) {
  pthread_t t_server, t_client_ic1, t_client_ic2;
  struct cc_state ccs;

  /*sigint callback*/
  if(signal(SIGINT, signal_handler) == SIG_ERR){
    puts("signal handler refused");
    return 1;
  }

  /*initialising internal intersection state*/
  cc_state_init(&ccs);

  /*initiailising the ncurses gui*/
  gui_init();

  /*initialising the clients for ic1, ic2*/
  struct client_init_data cid_ic1 = {
    .ccs = &ccs,
    .ci  = &ccs.ci_ic1
  };
  struct client_init_data cid_ic2 = {
    .ccs = &ccs,
    .ci  = &ccs.ci_ic2
  };
  pthread_create(&t_client_ic1, NULL, client_init, &cid_ic1);
  pthread_create(&t_client_ic2, NULL, client_init, &cid_ic2);

  /*initialising the server structure*/
  pthread_create(&t_server, NULL, server_init, &ccs);

  /*drawing the gui*/
  while(1){
    gui_solve(&ccs);
  }

  /*waiting for the server thread to join*/
  pthread_join(t_server, NULL);

  /*joining client threads*/
  pthread_join(t_client_ic1, NULL);
  pthread_join(t_client_ic2, NULL);

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
