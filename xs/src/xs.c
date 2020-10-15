// InterProcess Communication
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "messages.h"
#include <sys/iofunc.h>
#include <sys/netmgr.h>
#include <sys/dispatch.h>

const char* Name = "xs";
const char* ATTACH_POINT = "/net/x/dev/name/local/xc";

int main(int argc, char *argv[])
{
	printf("Sensor %s running\n", Name);

	// Just to check if Sensor is connected to xc
	name_attach_t *attach;
	if((attach = name_attach(NULL, Name, 0)) == NULL){
		printf("failed to attach name\n");
	    return EXIT_FAILURE;
	}

    int print_err_conn = 0;
	while(1)
	{
		int server_coid = name_open(ATTACH_POINT, 0);
		if(server_coid == -1)
		{
			if(print_err_conn == 0){
				printf("Failed to connect - Please check VM connections with (%s)\n", ATTACH_POINT);
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
		struct message reply;
		msg.pulse.type = 0x22;



		int successEnter = 0;
		do // Loop to make sure that the user enter valid value
		{
			char enter[1];
			printf("Enter the sensor state (1=Active): \n");
			scanf("%s", enter);

			if(strcmp(enter,"1") == 0)
			{
				msg.type = MESSAGE_SENSOR_TRAIN;
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
			printf("Error data (%s) NOT sent to server\n", message_type_string(msg.type));
			continue;
		}
	}
	printf("Sensor %s terminated\n", Name);
	return EXIT_SUCCESS;
}




