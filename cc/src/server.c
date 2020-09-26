#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/iofunc.h>
#include <errno.h>

#include "server.h"

int server_init(struct server *s){
  /*getting pid*/
  s->pid = getpid();

  /*creating channel*/
  if((s->chid = ChannelCreate(_NTO_CHF_DISCONNECT)) < 1){
    puts("failed to create channel");
    return 1;
  }

  /*debug displaying pid and chid*/
  printf(
    " server pid: %d\n"
    "server chid: %d\n",
    s->pid,
    s->chid
  );

  /*writing pid and chid to file for client*/
  FILE *ofp;
  if((ofp = fopen("/tmp/major_project/cc_server.info", "wb")) == NULL){
    puts("failed to open server.info file!");
    return 1;
  }
  fprintf(ofp, "%d\n%d\n", s->pid, s->chid);
  fclose(ofp);

  return 0;
}

int server_destroy(struct server *s){
  /*destroying the communication channel*/
  ChannelDestroy(s->chid);

  /*removing the server info file*/
  if(remove("/tmp/major_project/cc_server.info")){
    puts("failed to remove server.info file!");
    return 1;
  }

  return 0;
}
