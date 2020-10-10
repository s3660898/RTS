#ifndef MESSAGES_H
#define MESSAGES_H

/*for the pulse _struct*/
#include <sys/neutrino.h>

#include "intersection.h"

/*just a test string for an example of the shared header*/
#define MESSAGE_STRING "Hello there from the included header"

/* all of the possible message types, used to determine the information
 * that is sent/read from the 'data' variable in the 'message' struct
 */
enum message_type{

  /*for reply messages*/
  MESSAGE_REPLY_OK,
  MESSAGE_REPLY_FAIL,

  /*state (light or actual state) change messages*/
  MESSAGE_INTERSECTION_STATE_CHANGE,
  MESSAGE_PEDESTRIAN_LIGHT_CHANGE,
  MESSAGE_BOOMGATE_CHANGE,

  /*intersection sensor to intersection controller messages*/
  MESSAGE_SENSOR_INTERSECTION_NSS,  /*a vehicle wants to go north or south straight*/
  MESSAGE_SENSOR_INTERSECTION_NST,  /*a vehicle wants to go north or south, right hand turning*/
  MESSAGE_SENSOR_INTERSECTION_EWS,  /*a vehicle wants to go east or west straight*/
  MESSAGE_SENSOR_INTERSECTION_EWT,  /*a vehicle wants to go east or west, right hand turning*/

  /*pedestrian sensor to pedestrian controller messages*/
  MESSAGE_SENSOR_PEDESTRIAN_NORTH,
  MESSAGE_SENSOR_PEDESTRIAN_EAST,
  MESSAGE_SENSOR_PEDESTRIAN_SOUTH,
  MESSAGE_SENSOR_PEDESTRIAN_WEST,

  /*train sensor to train controller messages*/
  MESSAGE_SENSOR_TRAIN
};

/*the union for the message's data*/
union message_data{
  struct intersection isection;
};

struct message{
  /*will have the _pulse struct here*/
  struct _pulse pulse;

  /*project-specific message type, tells you how to interperate rest of the info*/
  enum message_type type;

  union message_data data;
};

#endif
