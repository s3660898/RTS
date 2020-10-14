#ifndef CLIENTS_H
#define CLIENTS_H

#include "message.h"
#include "cc_state.h"

#define ATTACH_POINT_IC1 "/net/"
#define ATTACH_POINT_IC2 "/net/"

struct client_info{
  /*the server's attach point*/
  const char *attach_point;

  /*pointer to the cc_state*/
  struct cc_state *ccs;

  /*mutex for message sending checking*/
  pthread_mutex_t mutex;
  struct message message;
};

void *client_init(void *data);

#endif
