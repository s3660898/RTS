// InterProcess Communication
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "messages.h"
#include <string.h>
#include <sys/iofunc.h>
#include <sys/netmgr.h>
#include <sys/dispatch.h>
#include <unistd.h>

const char* ATTACH_POINT = "/net/p2/dev/name/local/pc2";

int main(int argc, char *argv[])
{
  unsigned connected = 0;
  int server_coid = 0;
	while(1)
	{
    if(connected == 0){
      server_coid = name_open(ATTACH_POINT, 0);
      if(server_coid < 0)
      {
        printf("Failed to connect - Please check VM connections\n");
        sleep(1);
      }
      else
      {
        printf("Connection Established with intersection\n");
        connected = 1;
      }
    }

		struct message msg;
		struct message reply;
		msg.pulse.type = 0x22;

    if(connected){
      int successEnter = 0;
      do // Loop to make sure that the user enter valid value
      {
        printf("Enter the sensor state (1=NORTH, 2=EAST, 3=SOUTH, 4=WEST): \n");
        char enter[1];
        scanf("%s", enter);

        if(strcmp(enter,"1") == 0)
        {
          msg.type = MESSAGE_SENSOR_PEDESTRIAN_NORTH;
          successEnter = 1;
        }
        else if(strcmp(enter,"2") == 0)
        {
          msg.type = MESSAGE_SENSOR_PEDESTRIAN_EAST;
          successEnter = 1;
        }
        else if(strcmp(enter,"3") == 0)
        {
          msg.type = MESSAGE_SENSOR_PEDESTRIAN_SOUTH;
          successEnter = 1;
        }
        else if(strcmp(enter,"4") == 0)
        {
          msg.type = MESSAGE_SENSOR_PEDESTRIAN_WEST;
          successEnter = 1;
        }
        else
          printf("ERROR: Invalid sensor value.\n");
      } while(!successEnter);

      if (MsgSend(server_coid, &msg, sizeof(msg), &reply, sizeof(reply)) != -1)
      {
        printf("Success in sending sensor message (%s)\n", message_type_string(msg.type));

        if(reply.type == MESSAGE_REPLY_FAIL)
        {
          printf("Failed - Server replied with (%s) - Please check with Intersection\n", message_type_string(reply.type));
        }
        else
        {
          printf("Server replied with (%s)\n", message_type_string(reply.type));
        }
      }
      else
      {
        connected = 0;
        printf("Error data (%s) NOT sent to server\n", message_type_string(msg.type));
        continue;
      }
    }
  }
	printf("Sensor terminated\n");
	return EXIT_SUCCESS;
}

