#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include "messages.h"

#include "server.h"
#include "cc_state.h"

#define ATTACH_POINT "cc"

void *server_init(void *data){
  struct cc_state *ccs= (struct cc_state *)data;

  /*creating global name*/
  name_attach_t *attach;
  if((attach = name_attach(NULL, ATTACH_POINT, 0)) == NULL){
    puts("failed to attach name");
    return NULL;
  }

  /*recieving message loop*/
  unsigned running = 1;
  while(running){
    struct message msg;

    int rcvid = MsgReceive(attach->chid, &msg, sizeof(msg), NULL);

    /*MsgRecieve error*/
    if(rcvid < 0){
      puts("MsgRecieve error");
    }

    /*MsgRecieve pulse recieved*/
    else if(rcvid == 0){
      /*TODO, does nothing for now*/
    }

    /*MsgRecieved actual message recieved*/
    else{

      /*a global name service connect request*/
      if(msg.pulse.type == _IO_CONNECT){
        MsgReply(rcvid, EOK, NULL, 0);
      }

      /*rejecting other I/O messages*/
      else if(msg.pulse.type > _IO_BASE && msg.pulse.type <= _IO_MAX){
        ;
      }

      /*a project-related message*/
      else{
        struct message reply;
        int send_reply_ok = 1;

        switch(msg.type){
          case MESSAGE_INTERSECTION_STATE_CHANGE_IC1:
            /*updating state by copying message data*/
            pthread_mutex_lock(&ccs->mutex);
            ccs->i1_time = clock();
            memcpy(&ccs->i1, &msg.data, sizeof(struct intersection));
            pthread_mutex_unlock(&ccs->mutex);
            break;

          case MESSAGE_INTERSECTION_STATE_CHANGE_IC2:
            pthread_mutex_lock(&ccs->mutex);
            ccs->i2_time = clock();
            memcpy(&ccs->i2, &msg.data, sizeof(struct intersection));
            pthread_mutex_unlock(&ccs->mutex);
            break;

          case MESSAGE_PEDESTRIAN_LIGHT_CHANGE_PC1:
            pthread_mutex_lock(&ccs->mutex);
            ccs->p2_time = clock();
            memcpy(&ccs->p1, &msg.data.pcrossings, sizeof(struct pedestrian_crossings));
            pthread_mutex_unlock(&ccs->mutex);
            break;

          case MESSAGE_PEDESTRIAN_LIGHT_CHANGE_PC2:
            pthread_mutex_lock(&ccs->mutex);
            ccs->p1_time = clock();
            memcpy(&ccs->p2, &msg.data.pcrossings, sizeof(struct pedestrian_crossings));
            pthread_mutex_unlock(&ccs->mutex);
            break;

          case MESSAGE_BOOMGATE_RED:
            pthread_mutex_lock(&ccs->mutex);
            ccs->x_time = clock();
            ccs->x.state = TRAIN_CROSSING_RED;
            pthread_mutex_unlock(&ccs->mutex);
            break;

          case MESSAGE_BOOMGATE_GREEN:
            pthread_mutex_lock(&ccs->mutex);
            ccs->x_time = clock();
            ccs->x.state = TRAIN_CROSSING_GREEN;
            pthread_mutex_unlock(&ccs->mutex);
            break;

          default:
            reply.pulse.type = 0x01;
            reply.pulse.subtype = 0x00;
            reply.type = MESSAGE_REPLY_FAIL;
            MsgReply(rcvid, EOK, &reply, sizeof(struct message));

            send_reply_ok = 0;
        }

        if(send_reply_ok){
          /*preparing reply*/
          reply.pulse.type = 0x01;
          reply.pulse.subtype = 0x00;
          reply.type = MESSAGE_REPLY_OK;

          MsgReply(rcvid, EOK, &reply, sizeof(struct message));
        }
      }
    }

  }

  return NULL;
}
