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

#define ATTACH_POINT "dummy"

void *server_init(void *data){

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

        printf("message received, type: %s\n", message_type_string(msg.type));

        /*preparing reply*/
        reply.pulse.type = 0x01;
        reply.pulse.subtype = 0x00;
        reply.type = MESSAGE_REPLY_OK;

        MsgReply(rcvid, EOK, &reply, sizeof(struct message));
      }
    }

  }

  return NULL;
}
