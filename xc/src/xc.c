#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <ncurses.h>
#include <time.h>
#include<sys/netmgr.h>
#include<sys/neutrino.h>
#include <unistd.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include "messages.h"

const int TIMER_INTERVAL = 5;

#define LOCAL_ATTACH_POINT "xc"
#define ATTACH_POINT_CC "/net/VM_x86_Target02/dev/name/local/cc" // For Server
#define ATTACH_POINT_IC1 "/net/VM_x86_Target02/dev/name/local/ic1" // For IC1
#define ATTACH_POINT_IC2 "/net/VM_x86_Target02/dev/name/local/ic2" // For IC2
#define ATTACH_POINT_XS1 "/net/VM_x86_Target01/dev/name/local/xs" // For XS1

int timer_ch_id;

struct xc_state{

  timer_t timer;                 /*time since last message received*/

  /*multithreading related*/
  pthread_mutex_t mutex;
};
struct msg_param{
  char* attach_point;
  int type;
};

void* receive_sensor(void *data);
void* send_msg(void *);

void state_init(struct xc_state *xcs);
void thread_to_send_msg(struct msg_param* param);
void run_timer(timer_t timer);
void init_timer(struct xc_state *xcs);
int check_all_connections();

int main(void) {
  	printf("XC Running!!!\n");

	pthread_t server_thread;
	struct xc_state xcs;
	state_init(&xcs);
	init_timer(&xcs);

  	printf("Initialization Done!!!\n");


    /*initialising the server structure*/
    pthread_create(&server_thread, NULL, receive_sensor, &xcs);
  	printf("Thread created successfully!!!\n");


    /*waiting for the server thread to join*/
    pthread_join(server_thread, NULL);

    printf("XC Terminated!!!\n"); /* prints Hello World!!! */
	return EXIT_SUCCESS;
}

void* receive_sensor(void *data){
	printf("XC receive message started\n");

    struct xc_state *xcs = (struct xc_state *)data;

	  /*creating global name*/
	  name_attach_t *attach;
	  if((attach = name_attach(NULL, LOCAL_ATTACH_POINT, 0)) == NULL){
	    puts("failed to attach name\n");
		return NULL;
	  }
	  /*recieving message loop*/
	  unsigned running = 1;
	  int print_err_conn = 0;
	  while(running){
		int connected = check_all_connections();
		if(connected == -1)
		{
			if(print_err_conn == 0){
				printf("Failed to connect - Please check VM connections\n");
				print_err_conn = 1;
			}
			continue;
		}
		if(print_err_conn == 1)
		{
			printf("Connection Established with intersection\n");
			print_err_conn = 0;
		}
		struct message msg;

		int rcvid = MsgReceive(attach->chid, &msg, sizeof(msg), NULL);

		/*MsgRecieve error*/
		if(rcvid < 0){
			printf("Error in connecting to XS1 - Please check VM Connectivity\n");
			continue;
		}

		/*MsgRecieve pulse recieved*/
		else if(rcvid == 0){
		  /*TODO, does nothing for now*/
			continue;
		}

		printf("\nXC Received message (%s) from (coid = %d)!\n", message_type_string(msg.type) , rcvid);


		/*MsgRecieved actual message recieved*/
		struct message reply;

		switch(msg.type){
		  case MESSAGE_SENSOR_TRAIN:
			/*preparing reply*/
			reply.pulse.type = 0x01;
			reply.pulse.subtype = 0x00;
			reply.type = MESSAGE_REPLY_OK;

			MsgReply(rcvid, EOK, &reply, sizeof(struct message));

			struct msg_param param = {ATTACH_POINT_IC1, MESSAGE_BOOMGATE_RED};
			thread_to_send_msg(&param);

			param.attach_point = ATTACH_POINT_IC2;
			thread_to_send_msg(&param);

			param.attach_point = ATTACH_POINT_CC;
			thread_to_send_msg(&param);

			run_timer(xcs->timer);

			param.attach_point = ATTACH_POINT_IC1;
			param.type = MESSAGE_BOOMGATE_GREEN;
			thread_to_send_msg(&param);

			param.attach_point = ATTACH_POINT_IC2;
			thread_to_send_msg(&param);

			param.attach_point = ATTACH_POINT_CC;
			thread_to_send_msg(&param);
			break;

		  default:
			reply.pulse.type = 0x01;
			reply.pulse.subtype = 0x00;
			reply.type = MESSAGE_REPLY_FAIL;
			MsgReply(rcvid, EOK, &reply, sizeof(struct message));
		}
	  }
		printf("XC receive message Stopped\n");

	  return NULL;
}

void thread_to_send_msg(struct msg_param* param){
	printf("    ---- Thread to send msg to (%s)\n", param->attach_point);
	pthread_t thread;
    pthread_create(&thread, NULL, send_msg, param);
}

void* send_msg(void *data){
  struct msg_param *param = (struct msg_param *)data;
	struct message msg;
	struct message reply;
	msg.pulse.type = 0x22;
	msg.type =  param->type;
	char* point = param->attach_point;

	int controller_coid;
	if ((controller_coid = name_open(point, 0)) != -1)
	{
		printf("XC connected to Controller '%s' (coid = %d)!\n", point , controller_coid);
	}
	else
	{
		printf("ERROR, could not connect to the controller (%s)!\n", point);
		return NULL;
	}

	if (MsgSend(controller_coid, &msg, sizeof(msg), &reply, sizeof(reply)) != -1)
	{
		printf("Success in sending message (%s)\n", message_type_string(msg.type));

		if(reply.type == MESSAGE_REPLY_FAIL)
		{
			printf("Failed - Controller replied with (%s) - Please check with the controller (%s)\n", message_type_string(reply.type), param->attach_point);
		}
		else
		{
			printf("Controller replied with (%s)\n", message_type_string(reply.type));
		}
	}
	else
	{
		printf("Error data (%s) NOT sent to server\n", message_type_string(msg.type));
	}
	return NULL;
}


void state_init(struct xc_state *xcs){
  pthread_mutex_init(&xcs->mutex, NULL);

  xcs->timer = 0;
}


void init_timer(struct xc_state *xcs) {
	  /*3 seconds repeat timer*/
	  struct         sigevent event;    /*event*/
	  timer_t        timer;             /*timer id*/

	  /*creating self-connection*/
	  timer_ch_id = ChannelCreate(0);
	  event.sigev_notify = SIGEV_PULSE;
	  if((event.sigev_coid = ConnectAttach(ND_LOCAL_NODE, 0, timer_ch_id, _NTO_SIDE_CHANNEL, 0)) < 0){
	    puts("ConnectAttach failed.");
	  }
	  /*getting thread parameters*/
	  struct sched_param t_param;
	  pthread_getschedparam(pthread_self(), NULL, &t_param);
	  event.sigev_priority = t_param.sched_curpriority;
	  event.sigev_code = _PULSE_CODE_MINAVAIL;

	  /*creating timer and binding it to sigevent*/
	  if(timer_create(CLOCK_REALTIME, &event, &timer) < 0){
	    puts("timer_create failed!");
	  }

    xcs->timer = timer;
}

void run_timer(timer_t timer) {

  /*a non-repeating timespec*/
  const struct itimerspec its = {
    .it_value.tv_sec = TIMER_INTERVAL,
    .it_value.tv_nsec = 0,
    .it_interval.tv_sec = 0,
    .it_interval.tv_nsec = 0
  };

  timer_settime(timer, 0, &its, NULL);

  /*waiting for the timer*/
	struct _pulse p;
	while(1)
	{
		if(MsgReceive(timer_ch_id, &p, sizeof(struct _pulse), NULL) == 0 && p.code == _PULSE_CODE_MINAVAIL)
		{
			break;
		}
	}

	printf("Waited (%d) seconds\n", TIMER_INTERVAL);
}


int check_all_connections()
{
	int ic1_conn = name_open(ATTACH_POINT_IC1, 0);
	int ic2_conn = name_open(ATTACH_POINT_IC2, 0);
	int xs1_conn = name_open(ATTACH_POINT_XS1, 0);
	//printf("xs1_conn = %d\n", xs1_conn);
	//sleep(1);
	if((ic1_conn != -1 || ic2_conn != -1) && xs1_conn != -1) // Change to && if we need both ic1 and ic2 connected
		return 1;
	else return -1;
}




