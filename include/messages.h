#ifndef MESSAGES_H
#define MESSAGES_H

/*for the pulse _struct*/
#include <sys/neutrino.h>

#include "intersection.h"
#include "pedestrian.h"

/*just a test string for an example of the shared header*/
#define MESSAGE_STRING "Hello there from the included header"

/* all of the possible message types, used to determine the information
 * that is sent/read from the 'data' variable in the 'message' struct
 */
enum message_type{

  /*for reply messages*/
  MESSAGE_REPLY_OK,
  MESSAGE_REPLY_FAIL,

  /*local controller mode changes*/
  MESSAGE_SET_MODE_FIXED,
  MESSAGE_SET_MODE_SENSOR,
  MESSAGE_SET_MODE_MANUAL,

  /*state (light or actual state) change messages*/
  MESSAGE_INTERSECTION_STATE_CHANGE_IC1,
  MESSAGE_INTERSECTION_STATE_CHANGE_IC2,
  MESSAGE_PEDESTRIAN_LIGHT_CHANGE_PC1,
  MESSAGE_PEDESTRIAN_LIGHT_CHANGE_PC2,
  MESSAGE_BOOMGATE_GREEN,
  MESSAGE_BOOMGATE_RED,

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
  MESSAGE_SENSOR_TRAIN,

  /*cc -> icx setting intersection state manually*/
  MESSAGE_SET_STATE_MANUAL
};

static inline const char *message_type_string(enum message_type mt){
  switch(mt){
    case MESSAGE_REPLY_OK:
      return "MESSAGE_REPLY_OK";
    case MESSAGE_REPLY_FAIL:
      return "MESSAGE_REPLY_FAIL";

    case MESSAGE_SET_MODE_FIXED:
      return "MESSAGE_SET_MODE_FIXED";
    case MESSAGE_SET_MODE_SENSOR:
      return "MESSAGE_SET_MODE_SENSOR";
    case MESSAGE_SET_MODE_MANUAL:
      return "MESSAGE_SET_MODE_MANUAL";

    case MESSAGE_INTERSECTION_STATE_CHANGE_IC1:
      return "MESSAGE_INTERSECTION_STATE_CHANGE_IC1";
    case MESSAGE_INTERSECTION_STATE_CHANGE_IC2:
      return "MESSAGE_INTERSECTION_STATE_CHANGE_IC2";
    case MESSAGE_PEDESTRIAN_LIGHT_CHANGE_PC1:
      return "MESSAGE_PEDESTRIAN_LIGHT_CHANGE_PC1";
    case MESSAGE_PEDESTRIAN_LIGHT_CHANGE_PC2:
      return "MESSAGE_PEDESTRIAN_LIGHT_CHANGE_PC2";
    case MESSAGE_BOOMGATE_GREEN:
      return "MESSAGE_BOOMGATE_GREEN";
    case MESSAGE_BOOMGATE_RED:
      return "MESSAGE_BOOMGATE_RED";


    case MESSAGE_SENSOR_INTERSECTION_NSS:
      return "MESSAGE_SENSOR_INTERSECTION_NSS";
    case MESSAGE_SENSOR_INTERSECTION_NST:
      return "MESSAGE_SENSOR_INTERSECTION_NST";
    case MESSAGE_SENSOR_INTERSECTION_EWS:
      return "MESSAGE_SENSOR_INTERSECTION_EWS";
    case MESSAGE_SENSOR_INTERSECTION_EWT:
      return "MESSAGE_SENSOR_INTERSECTION_EWT";

    case MESSAGE_SENSOR_PEDESTRIAN_NORTH:
      return "MESSAGE_SENSOR_PEDESTRIAN_NORTH";
    case MESSAGE_SENSOR_PEDESTRIAN_EAST:
      return "MESSAGE_SENSOR_PEDESTRIAN_EAST";
    case MESSAGE_SENSOR_PEDESTRIAN_SOUTH:
      return "MESSAGE_SENSOR_PEDESTRIAN_SOUTH";
    case MESSAGE_SENSOR_PEDESTRIAN_WEST:
      return "INTERSECTION_STATE_EWS_Y";

    case MESSAGE_SENSOR_TRAIN:
      return "MESSAGE_SENSOR_TRAIN";

    case MESSAGE_SET_STATE_MANUAL:
      return "MESSAGE_SET_STATE_MANUAL";

    default:
      return "N.A";
  }
}

/*the union for the message's data*/
union message_data{
  struct intersection isection;
  enum intersection_state isection_state;
  struct pedestrian_crossings pcrossings;
};

struct message{
  /*will have the _pulse struct here*/
  struct _pulse pulse;

  /*project-specific message type, tells you how to interperate rest of the info*/
  enum message_type type;

  union message_data data;
};

#endif
