#ifndef MESSAGES_H
#define MESSAGES_H

#include "intersection.h"

/*just a test string for an example of the shared header*/
#define MESSAGE_STRING "Hello there from the included header"

/* all of the possible message types, used to determine the information
 * that is sent/read from the 'data' variable in the 'message' struct
 */
enum message_type{

  /*state (light or actual state) change messages*/
  MESSAGE_INTERSECTION_STATE_CHANGE = 0,
  MESSAGE_INTERSECTION_LIGHT_CHANGE = 1,
  MESSAGE_PEDESTRIAN_LIGHT_CHANGE   = 2,
  MESSAGE_BOOMGATE_CHANGE           = 3,

  /*intersection sensor to intersection controller messages*/
  MESSAGE_SENSOR_INTERSECTION_NSS   = 4,  /*a vehicle wants to go north or south straight*/
  MESSAGE_SENSOR_INTERSECTION_NST   = 5,  /*a vehicle wants to go north or south, right hand turning*/
  MESSAGE_SENSOR_INTERSECTION_EWS   = 6,  /*a vehicle wants to go east or west straight*/
  MESSAGE_SENSOR_INTERSECTION_EWT   = 7,  /*a vehicle wants to go east or west, right hand turning*/

  /*pedestrian sensor to pedestrian controller messages*/
  MESSAGE_SENSOR_PEDESTRIAN_NORTH   = 8,
  MESSAGE_SENSOR_PEDESTRIAN_EAST    = 9,
  MESSAGE_SENSOR_PEDESTRIAN_SOUTH   = 10,
  MESSAGE_SENSOR_PEDESTRIAN_WEST    = 11,

  /*train sensor to train controller messages*/
  MESSAGE_SENSOR_TRAIN              = 12
};

struct message_data_intersection_state{
  enum intersection_state i_state;
};

/*the union for the message's data*/
union message_data{
  struct message_data_intersection_state i_state;
};

struct message{
  /*will have the _pulse struct here*/

  /*message type, tells you how to interperate rest of the info*/
  enum message_type type;

  union message_data data;
};

#endif
