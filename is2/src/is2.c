// InterProcess Communication
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "messages.h"
#include <string.h>
#include <sys/iofunc.h>
#include <sys/netmgr.h>
#include <sys/dispatch.h>

const char* Name = "is2";
const char* ATTACH_POINT = "/net/VM_x86_Target02/dev/name/local/ic2";

int main(int argc, char *argv[])
{
	printf("Sensor %s running\n", Name);

    int print_err_conn = 0;
	while(1)
	{
		int server_coid = name_open(ATTACH_POINT, 0);
		if(server_coid == -1)
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
		struct message reply;
		msg.pulse.type = 0x22;

		int successEnter = 0;
		do // Loop to make sure that the user enter valid value
		{
			printf("Enter the sensor state (1=NSS, 2=NST, 3=EWS, 4=EWT): \n");
			char enter[1];
			scanf("%s", enter);

			if(strcmp(enter,"1") == 0)
			{
				msg.type = MESSAGE_SENSOR_INTERSECTION_NSS;
				successEnter = 1;
			}
			else if(strcmp(enter,"2") == 0)
			{
				msg.type = MESSAGE_SENSOR_INTERSECTION_NST;
				successEnter = 1;
			}
			else if(strcmp(enter,"3") == 0)
			{
				msg.type = MESSAGE_SENSOR_INTERSECTION_EWS;
				successEnter = 1;
			}
			else if(strcmp(enter,"4") == 0)
			{
				msg.type = MESSAGE_SENSOR_INTERSECTION_EWT;
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

