#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
  ci->send_message = 0;
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

    /*caching the shared memory flag*/
    pthread_mutex_lock(&ci->mutex);
    int send_message = ci->send_message;
    pthread_mutex_unlock(&ci->mutex);

    if(send_message){
      /*preparing the message struct*/
      struct message msg, reply;
      msg.pulse.type = 0x22;

      /*copying the message to local struct from ci*/
      pthread_mutex_lock(&ci->mutex);
      msg.type = ci->msg.type;
      memcpy(&msg.data, &ci->msg.data, sizeof(union message_data));
      pthread_mutex_unlock(&ci->mutex);

      /*sending the message and handling response/failure*/
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

      /*deaserting the send_message flag*/
      pthread_mutex_lock(&ci->mutex);
      ci->send_message = 0;
      pthread_mutex_unlock(&ci->mutex);
    }

  }

  puts("exiting successfully");
  return NULL;
}

void client_send_message(struct client_info *ci, enum message_type type, void *data, size_t n){
  pthread_mutex_lock(&ci->mutex);

  memcpy(&ci->msg, data, n);
  ci->msg.type = type;
  ci->send_message = 1;

  pthread_mutex_unlock(&ci->mutex);
}
