#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>

#include "clients.h"

void client_connect(struct client_info *ci){
  /*opening global name*/
  if((ci->coid = name_open(ci->attach_point, 0)) == -1){
    ci->connection_state = CONNECTION_STATE_DISCONNECTED;
    return;
  }

  ci->connection_state = CONNECTION_STATE_CONNECTED;
}

void client_info_init(struct client_info *ci, const char *attach_point){
  ci->attach_point = attach_point;
  ci->connection_state = CONNECTION_STATE_DISCONNECTED;
  pthread_mutex_init(&ci->mutex, NULL);
}

void *client_init(void *data){

  /*unwrapping data*/
  struct client_init_data *cid = (struct client_init_data *)data;
#if 0
  struct cc_state *ccs = (struct cc_state *)cid->ccs;
#endif
  struct client_info *ci = (struct client_info *)cid->ci;


  /*sending messages in a loop*/
  unsigned running = 1;
  while(running){

    /*automatic reconnect attampts if not connected*/
    if(ci->connection_state == CONNECTION_STATE_DISCONNECTED)
      client_connect(ci);

    /*preparing the message struct*/
    struct message msg, reply;

    msg.pulse.type = 0x22;
    msg.type       = MESSAGE_INTERSECTION_STATE_CHANGE_IC1;

    if(MsgSend(ci->coid, &msg, sizeof(struct message), &reply, sizeof(struct message)) == -1){

      ci->connection_state = CONNECTION_STATE_DISCONNECTED;

    }else{

#if 0
      /*printing reply type*/
      switch(reply.type){
        case MESSAGE_REPLY_OK:
          puts("MESSAGE_REPLY_OK received");
          break;
        case MESSAGE_REPLY_FAIL:
          puts("MESSAGE_REPLY_FAIL received");
          break;

        default:
          puts("unknown reply received");
      }
#endif

    }

  }

  puts("exiting successfully");
  return NULL;
}
