#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <errno.h>

#include "messages.h"

#include "server.h"

#define ATTACH_POINT "cc"

int server_init(struct server *s){

  /*creating global name*/
  name_attach_t *attach;
  if((attach = name_attach(NULL, ATTACH_POINT, 0)) == NULL){
    puts("failed to attach name");
    return 1;
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

      else{
        if(msg.type == MESSAGE_INTERSECTION_STATE_CHANGE){

          /*preparing reply*/
          struct message reply;
          reply.pulse.type = 0x01;
          reply.pulse.subtype = 0x00;
          reply.type = MESSAGE_REPLY_OK;

          MsgReply(rcvid, EOK, &reply, sizeof(struct message));
        }
      }
    }

  }

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
