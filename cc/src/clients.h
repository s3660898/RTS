#ifndef CLIENTS_H
#define CLIENTS_H

#include "messages.h"

#define ATTACH_POINT_IC1 "/net/VM_x86_Target01/dev/name/local/ic1"
#define ATTACH_POINT_IC2 "/net/VM_x86_Target01/dev/name/local/ic2"

enum connection_state{
  CONNECTION_STATE_CONNECTED,
  CONNECTION_STATE_DISCONNECTED
};

static inline const char *connection_state_string(enum connection_state cs){
  switch(cs){
    case CONNECTION_STATE_CONNECTED:
      return "CONNECTED";
    case CONNECTION_STATE_DISCONNECTED:
      return "DISCONNECTED (ATTEMPTING RECONNECT)";
  }
  return NULL;
}

struct client_info{
  /*the server's attach point*/
  const char *attach_point;

  /*channel id*/
  int coid;

  /*1 connected, 0 no connection*/
  enum connection_state connection_state;

  /*message for message sending*/
  struct message msg;
  int send_message;

  /*structure mutex*/
  pthread_mutex_t mutex;
};

#include "cc_state.h"

void client_info_init(struct client_info *ci, const char *attach_point);

struct client_init_data{
  struct client_info *ci;
  struct cc_state *ccs;
};

void *client_init(void *data);

void client_send_message(struct client_info *ci, enum message_type mt, void *data, size_t n);

#endif
