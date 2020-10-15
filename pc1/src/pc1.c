#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/netmgr.h>
#include <sys/neutrino.h>


#include "messages.h"
#include "pedestrian.h"

#define TIMER_INTERVAL 1

#define ATTACH_POINT "pc1"
const char* ic1 = "/net/i1/dev/name/local/ic1";
const char* cc  = "/net/cc/dev/name/local/cc";


int dataReadyCC = 0;
int dataReadyIC = 0;

struct pedestrian_crossing_shared{
  struct pedestrian_crossings pc;
  int passCCFlag;
  int passICFlag;
  pthread_mutex_t mutex;
};
struct pedestrian_crossing_shared pcs;



unsigned int sensorState = 0;
/*TODO change back to 0*/
unsigned int interState = 3;



int timer_ch_id;

struct xc_state{

  timer_t timer;                 //time since last message received

  //multithreading related
  pthread_mutex_t mutex;
};


void init_timer(struct xc_state *xcs) {
	  //3 seconds repeat timer
	  struct         sigevent event;    //event
	  timer_t        timer;             //timer id

	  //creating self-connection
	  timer_ch_id = ChannelCreate(0);
	  event.sigev_notify = SIGEV_PULSE;
	  if((event.sigev_coid = ConnectAttach(ND_LOCAL_NODE, 0, timer_ch_id, _NTO_SIDE_CHANNEL, 0)) < 0){
	    puts("ConnectAttach failed.");
	  }
	  //getting thread parameters
	  struct sched_param t_param;
	  pthread_getschedparam(pthread_self(), NULL, &t_param);
	  event.sigev_priority = t_param.sched_curpriority;
	  event.sigev_code = _PULSE_CODE_MINAVAIL;

	  //creating timer and binding it to sigevent
	  if(timer_create(CLOCK_REALTIME, &event, &timer) < 0){
	    puts("timer_create failed!");
	  }

    xcs->timer = timer;
}

void run_timer(timer_t timer) {

  //a non-repeating timespec
  const struct itimerspec its = {
    .it_value.tv_sec = TIMER_INTERVAL,
    .it_value.tv_nsec = 0,
    .it_interval.tv_sec = 0,
    .it_interval.tv_nsec = 0
  };

  //starting the timer
  timer_settime(timer, 0, &its, NULL);

  //waiting for the timer
	struct _pulse p;
	while(1)
	{
		if(MsgReceive(timer_ch_id, &p, sizeof(struct _pulse), NULL) == 0 && p.code == _PULSE_CODE_MINAVAIL)
		{
			break;
		}
	}
}




//Thread for message passing to CC

void *th_cc_passing (void *data){
  int server_coid;

  unsigned int running = 1;
  unsigned connected = 0;

  while(running){

    if(!connected){
      if((server_coid = name_open(cc, 0)) < 0){
        sleep(1);
        puts("failed to open global name for CC");
      }else{
        connected = 1;
      }
    }

    //preparing the message struct
    struct message msg, reply;

    msg.pulse.type = 0x22;

    if(connected && pcs.passCCFlag == 1){


      msg.type = MESSAGE_PEDESTRIAN_LIGHT_CHANGE_PC1;

      memcpy(&msg.data.pcrossings, &pcs.pc, sizeof(struct pedestrian_crossings));

      puts("Sending");

      if (MsgSend(server_coid, &msg, sizeof(struct message), &reply, sizeof(struct message)) < 0){
        puts("MsgSend failed");
        connected = 0;
      }

      else{

      //printing reply type
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

      pthread_mutex_lock(&pcs.mutex);
      pcs.passCCFlag = 0;
      pthread_mutex_unlock(&pcs.mutex);
    }
  }


	puts("exiting successfully");
	return 0;

}

//Thread for msg passing to IC1

void *th_ic1_passing (void *data){




		int server_coid;

		//opening global name






	//sending messages in a loop

		unsigned int running = 1;
    unsigned connected = 0;

		while(running){

      if(!connected){
        if((server_coid = name_open(ic1, 0)) == -1){
          sleep(1);
          puts("failed to open global name for IC1");
        }else{
          connected = 1;
        }
      }

		//preparing the message struct
		struct message msg, reply;



		msg.pulse.type = 0x22;



		if(connected && pcs.passICFlag == 1){



		switch(sensorState){

				case 1:
					msg.type = MESSAGE_SENSOR_PEDESTRIAN_NORTH;
					break;

				case 2:
					msg.type = MESSAGE_SENSOR_PEDESTRIAN_EAST;
					break;

				case 3:
					msg.type = MESSAGE_SENSOR_PEDESTRIAN_WEST;
					break;

				case 4:
					msg.type = MESSAGE_SENSOR_PEDESTRIAN_SOUTH;
					break;
				}

		if(MsgSend(server_coid, &msg, sizeof(struct message), &reply, sizeof(struct message)) == -1){
      connected = 0;
			puts("MsgSend failed");
		}
		else{

			//printing reply type
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

		  pthread_mutex_lock(&pcs.mutex);
		  pcs.passICFlag = 0;
		  pthread_mutex_unlock(&pcs.mutex);
	}
	}
	puts("exiting successfully");
	return 0;

}




int main(void){



	pthread_mutex_init(&pcs.mutex, NULL);



	pcs.pc.north = PEDESTRIAN_CROSSING_RED;
	pcs.pc.east = PEDESTRIAN_CROSSING_RED;
	pcs.pc.west = PEDESTRIAN_CROSSING_RED;
	pcs.pc.south = PEDESTRIAN_CROSSING_RED;



	pthread_t th1;
	pthread_t th2;






	pthread_create (&th1, NULL, th_cc_passing, NULL);
	pthread_create (&th2, NULL, th_ic1_passing, NULL);


	struct xc_state xcs;
	init_timer(&xcs);

	name_attach_t *attach;
	  if((attach = name_attach(NULL, ATTACH_POINT, 0)) == NULL){
	    puts("failed to attach name");

	  }


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


	    	    	  switch(msg.type){



	    	    	  case MESSAGE_SENSOR_PEDESTRIAN_NORTH:


	    	    		  sensorState = 1;
	    	    		  pthread_mutex_lock(&pcs.mutex);
	    	    		  pcs.passICFlag = 1;
	    	    		  pthread_mutex_unlock(&pcs.mutex);


	    	    		  if (interState == 3){

	    	    			  pthread_mutex_lock(&pcs.mutex);
	    	    			  pcs.pc.north = PEDESTRIAN_CROSSING_GREEN;
	    	    			  pcs.passCCFlag = 1;
	    	    			  pthread_mutex_unlock(&pcs.mutex);

	    	    			  run_timer(xcs.timer);

	    	    			  pthread_mutex_lock(&pcs.mutex);
	    	    			  pcs.pc.north = PEDESTRIAN_CROSSING_YELLOW;
	    	    			  pcs.passCCFlag = 1;
	    	    			  pthread_mutex_unlock(&pcs.mutex);

	    	    			  run_timer(xcs.timer);


	    	    			  pthread_mutex_lock(&pcs.mutex);
	    	    			  pcs.pc.north = PEDESTRIAN_CROSSING_RED;
	    	    			  pcs.passCCFlag = 1;
	    	    			  pthread_mutex_unlock(&pcs.mutex);




	    	    		  }
	    	    		  /*preparing reply*/
	    	              reply.pulse.type = 0x01;
	    	              reply.pulse.subtype = 0x00;
	    	              reply.type = MESSAGE_REPLY_OK;

	    	              MsgReply(rcvid, EOK, &reply, sizeof(struct message));
	    	              break;

	    	    	  case MESSAGE_SENSOR_PEDESTRIAN_EAST:

	    	    		  sensorState = 2;
	    	    		  pthread_mutex_lock(&pcs.mutex);
	    	    		  pcs.passICFlag = 1;
	    	    		  pthread_mutex_unlock(&pcs.mutex);

	    	    		  if (interState == 1){

	    	    			  pthread_mutex_lock(&pcs.mutex);
	    	    			  pcs.pc.east = PEDESTRIAN_CROSSING_GREEN;
	    	    			  pcs.passCCFlag = 1;
	    	    			  pthread_mutex_unlock(&pcs.mutex);

	    	    			  run_timer(xcs.timer);


	    	    			  pthread_mutex_lock(&pcs.mutex);
	    	    			  pcs.pc.east = PEDESTRIAN_CROSSING_YELLOW;
	    	    			  pcs.passCCFlag = 1;
	    	    			  pthread_mutex_unlock(&pcs.mutex);

	    	    			  run_timer(xcs.timer);


	    	    			  pthread_mutex_lock(&pcs.mutex);
	    	    			  pcs.pc.east = PEDESTRIAN_CROSSING_RED;
	    	    			  pcs.passCCFlag = 1;
	    	    			  pthread_mutex_unlock(&pcs.mutex);


	    	    		  }

	    	              /*preparing reply*/
	    	              reply.pulse.type = 0x01;
	    	              reply.pulse.subtype = 0x00;
	    	              reply.type = MESSAGE_REPLY_OK;

	    	              MsgReply(rcvid, EOK, &reply, sizeof(struct message));
	    	              break;

	    	    	  case MESSAGE_SENSOR_PEDESTRIAN_WEST:

	    	    		  sensorState = 3;
	    	    		  pthread_mutex_lock(&pcs.mutex);
	    	    		  pcs.passICFlag = 1;
	    	    		  pthread_mutex_unlock(&pcs.mutex);

	    	    		  if (interState == 1){

	    	    			  pthread_mutex_lock(&pcs.mutex);
	    	    			  pcs.pc.west = PEDESTRIAN_CROSSING_GREEN;
	    	    			  pcs.passCCFlag = 1;
	    	    			  pthread_mutex_unlock(&pcs.mutex);

	    	    			  run_timer(xcs.timer);


	    	    			  pthread_mutex_lock(&pcs.mutex);
	    	    			  pcs.pc.west = PEDESTRIAN_CROSSING_YELLOW;
	    	    			  pcs.passCCFlag = 1;
	    	    			  pthread_mutex_unlock(&pcs.mutex);

	    	    			  run_timer(xcs.timer);


	    	    			  pthread_mutex_lock(&pcs.mutex);
	    	    			  pcs.pc.west = PEDESTRIAN_CROSSING_RED;
	    	    			  pcs.passCCFlag = 1;
	    	    			  pthread_mutex_unlock(&pcs.mutex);


	    	    		  }
	    	              /*preparing reply*/
	    	              reply.pulse.type = 0x01;
	    	              reply.pulse.subtype = 0x00;
	    	              reply.type = MESSAGE_REPLY_OK;

	    	              MsgReply(rcvid, EOK, &reply, sizeof(struct message));
	    	              break;

	    	    	  case MESSAGE_SENSOR_PEDESTRIAN_SOUTH:

	    	    		  sensorState = 4;
	    	    		  pthread_mutex_lock(&pcs.mutex);
	    	    		  pcs.passICFlag = 1;
	    	    		  pthread_mutex_unlock(&pcs.mutex);


	    	    		  if (interState == 3){
	    	    			  pthread_mutex_lock(&pcs.mutex);
	    	    			  pcs.pc.south = PEDESTRIAN_CROSSING_GREEN;
	    	    			  pcs.passCCFlag = 1;
	    	    			  pthread_mutex_unlock(&pcs.mutex);

	    	    			  run_timer(xcs.timer);



	    	    			  pthread_mutex_lock(&pcs.mutex);
	    	    			  pcs.pc.south = PEDESTRIAN_CROSSING_YELLOW;
	    	    			  pcs.passCCFlag = 1;
	    	    			  pthread_mutex_unlock(&pcs.mutex);

	    	    			  run_timer(xcs.timer);


	    	    			  pthread_mutex_lock(&pcs.mutex);
	    	    			  pcs.pc.south = PEDESTRIAN_CROSSING_RED;
	    	    			  pcs.passCCFlag = 1;
	    	    			  pthread_mutex_unlock(&pcs.mutex);


	    	    		  }

	    	              /*preparing reply*/
	    	              reply.pulse.type = 0x01;
	    	              reply.pulse.subtype = 0x00;
	    	              reply.type = MESSAGE_REPLY_OK;

	    	              MsgReply(rcvid, EOK, &reply, sizeof(struct message));
	    	              break;

	    	    	  case MESSAGE_INTERSECTION_STATE_CHANGE_IC1:


	    	    		  switch(msg.data.isection_state){

	    	    		  case INTERSECTION_STATE_NSS_G:
	    	    			  interState = 1;
	    	    			  if(sensorState == 2){

	    	    				  pthread_mutex_lock(&pcs.mutex);
	    	    				  pcs.pc.east = PEDESTRIAN_CROSSING_GREEN;
	    	    				  pcs.passCCFlag = 1;
		    	    			  pthread_mutex_unlock(&pcs.mutex);

		    	    			  run_timer(xcs.timer);

		    	    			  pthread_mutex_lock(&pcs.mutex);
		    	    			  pcs.pc.east = PEDESTRIAN_CROSSING_YELLOW;
		    	    			  pcs.passCCFlag = 1;
		    	    			  pthread_mutex_unlock(&pcs.mutex);

		    	    			  run_timer(xcs.timer);

		    	    			  pthread_mutex_lock(&pcs.mutex);
		    	    			  pcs.pc.east = PEDESTRIAN_CROSSING_RED;
		    	    			  pcs.passCCFlag = 1;
		    	    			  pthread_mutex_unlock(&pcs.mutex);



	    	    			  }

	    	    			  if(sensorState == 3){
	    	    				  pthread_mutex_lock(&pcs.mutex);
	    	    				  pcs.pc.west = PEDESTRIAN_CROSSING_GREEN;
	    	    				  pcs.passCCFlag = 1;
		    	    			  pthread_mutex_unlock(&pcs.mutex);


		    	    			  run_timer(xcs.timer);

		    	    			  pthread_mutex_lock(&pcs.mutex);
	    	    				  pcs.pc.west = PEDESTRIAN_CROSSING_YELLOW;
	    	    				  pcs.passCCFlag = 1;
		    	    			  pthread_mutex_unlock(&pcs.mutex);


		    	    			  run_timer(xcs.timer);

		    	    			  pthread_mutex_lock(&pcs.mutex);
	    	    				  pcs.pc.west = PEDESTRIAN_CROSSING_RED;
	    	    				  pcs.passCCFlag = 1;
		    	    			  pthread_mutex_unlock(&pcs.mutex);



	    	    			  }

		    	              /*preparing reply*/
		    	              reply.pulse.type = 0x01;
		    	              reply.pulse.subtype = 0x00;
		    	              reply.type = MESSAGE_REPLY_OK;

		    	              MsgReply(rcvid, EOK, &reply, sizeof(struct message));
		    	              break;


	    	    		  case INTERSECTION_STATE_NST_G:
	    	    			  interState = 2;

		    	              /*preparing reply*/
		    	              reply.pulse.type = 0x01;
		    	              reply.pulse.subtype = 0x00;
		    	              reply.type = MESSAGE_REPLY_OK;

		    	              MsgReply(rcvid, EOK, &reply, sizeof(struct message));
		    	              break;



	    	    		  case INTERSECTION_STATE_EWS_G:
	    	    			  interState = 3;

	    	    			  if(sensorState == 1){
	    	    				  pthread_mutex_lock(&pcs.mutex);
	    	    				  pcs.pc.north = PEDESTRIAN_CROSSING_GREEN;
	    	    				  pcs.passCCFlag = 1;
		    	    			  pthread_mutex_unlock(&pcs.mutex);


		    	    			  run_timer(xcs.timer);

	    	    				  pthread_mutex_lock(&pcs.mutex);
	    	    				  pcs.pc.north = PEDESTRIAN_CROSSING_YELLOW;
	    	    				  pcs.passCCFlag = 1;
		    	    			  pthread_mutex_unlock(&pcs.mutex);


		    	    			  run_timer(xcs.timer);

	    	    				  pthread_mutex_lock(&pcs.mutex);
	    	    				  pcs.pc.north = PEDESTRIAN_CROSSING_RED;
	    	    				  pcs.passCCFlag = 1;
		    	    			  pthread_mutex_unlock(&pcs.mutex);


	    	    			  }

	    	    			  if(sensorState == 4){
	    	    				  pthread_mutex_lock(&pcs.mutex);
	    	    				  pcs.pc.south = PEDESTRIAN_CROSSING_GREEN;
	    	    				  pcs.passCCFlag = 1;
		    	    			  pthread_mutex_unlock(&pcs.mutex);

		    	    			  run_timer(xcs.timer);

	    	    				  pthread_mutex_lock(&pcs.mutex);
	    	    				  pcs.pc.south = PEDESTRIAN_CROSSING_YELLOW;
	    	    				  pcs.passCCFlag = 1;
		    	    			  pthread_mutex_unlock(&pcs.mutex);

		    	    			  run_timer(xcs.timer);

	    	    				  pthread_mutex_lock(&pcs.mutex);
	    	    				  pcs.pc.south = PEDESTRIAN_CROSSING_RED;
	    	    				  pcs.passCCFlag = 1;
		    	    			  pthread_mutex_unlock(&pcs.mutex);


	    	    			  }

		    	              /*preparing reply*/
		    	              reply.pulse.type = 0x01;
		    	              reply.pulse.subtype = 0x00;
		    	              reply.type = MESSAGE_REPLY_OK;

		    	              MsgReply(rcvid, EOK, &reply, sizeof(struct message));
		    	              break;


	    	    		  case INTERSECTION_STATE_EWT_G:
	    	    			  interState = 4;

		    	              /*preparing reply*/
		    	              reply.pulse.type = 0x01;
		    	              reply.pulse.subtype = 0x00;
		    	              reply.type = MESSAGE_REPLY_OK;

		    	              MsgReply(rcvid, EOK, &reply, sizeof(struct message));
		    	              break;


	    	    		  }





	    	    	  }

	    	      }



	    }
	    }

	    printf("Server running\n");



	    return 0;


}








