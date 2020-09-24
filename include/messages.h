#ifndef MESSAGES_H
#define MESSAGES_H

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

/*enum of the different possible intersection states*/
enum intersection_state{

  /*north-south straight*/
  INTERSECTION_STATE_NSS_G,
  INTERSECTION_STATE_NSS_Y,
  INTERSECTION_STATE_NSS_R,

  /*north-south turn*/
  INTERSECTION_STATE_NST_G,
  INTERSECTION_STATE_NST_Y,
  INTERSECTION_STATE_NST_R,

  /*east-west straight*/
  INTERSECTION_STATE_EWS_G,
  INTERSECTION_STATE_EWS_Y,
  INTERSECTION_STATE_EWS_R,

  /*east-west turn*/
  INTERSECTION_STATE_EWT_G,
  INTERSECTION_STATE_EWT_Y,
  INTERSECTION_STATE_EWT_R
};

struct message_data_intersection_state{
  enum intersection_state i_state;
};

/*different possible intersection light states*/
enum light_state{
  LIGHT_RED,
  LIGHT_YELLOW,
  LIGHT_GREEN
};

enum road_direction{
  ROAD_NORTH,
  ROAD_EAST,
  ROAD_SOUTH,
  ROAD_WEST
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
