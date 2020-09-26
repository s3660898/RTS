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

  /*senor -> local controller messages*/
  MESSAGE_SENSOR_PEDESTRIAN         = 4,
  MESSAGE_SENSOR_CAR                = 5,
  MESSAGE_SENSOR_TRAIN              = 6

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
