#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/dispatch.h>

#include "messages.h"

#define ATTACH_POINT "/net/VM_x86_Target01/dev/name/local/cc"

int main(void) {


  int server_coid;

  /*seeding random number generator*/
  srand(time(NULL));

  /*opening global name*/
  if((server_coid = name_open(ATTACH_POINT, 0)) == -1){
    puts("failed to open global name");
    return 1;
  }

  /*sending messages in a loop*/
  unsigned i;
  for(i = 0; i < 5; i++){

    /*preparing the message struct*/
    struct message msg, reply;

    msg.pulse.type = 0x22;
    msg.type       = MESSAGE_INTERSECTION_STATE_CHANGE_IC1;
    intersection_init(&msg.data.isection);
#if 0
    msg.data.isection.state = INTERSECTION_STATE_NST_Y;
#endif

    /*generating random data*/
    msg.data.isection.state = rand()%12;

    msg.data.isection.road_north.left     = rand()%3;
    msg.data.isection.road_north.straight = rand()%3;
    msg.data.isection.road_north.right    = rand()%3;

    msg.data.isection.road_east.left     = rand()%3;
    msg.data.isection.road_east.straight = rand()%3;
    msg.data.isection.road_east.right    = rand()%3;

    msg.data.isection.road_south.left     = rand()%3;
    msg.data.isection.road_south.straight = rand()%3;
    msg.data.isection.road_south.right    = rand()%3;

    msg.data.isection.road_west.left     = rand()%3;
    msg.data.isection.road_west.straight = rand()%3;
    msg.data.isection.road_west.right    = rand()%3;


    if(MsgSend(server_coid, &msg, sizeof(struct message), &reply, sizeof(struct message)) == -1){

      puts("MsgSend failed");
      return 1;

    }else{

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

    }

    sleep(1);

  }

  puts("exiting successfully");
  return 0;
}
