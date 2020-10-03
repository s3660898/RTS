#ifndef INTERSECTION_H
#define INTERSECTION_H

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

static inline const char *intersection_state_string(enum intersection_state is){
  switch(is){
    case INTERSECTION_STATE_NSS_G:
      return "INTERSECTION_STATE_NSS_G";
    case INTERSECTION_STATE_NSS_Y:
      return "INTERSECTION_STATE_NSS_Y";
    case INTERSECTION_STATE_NSS_R:
      return "INTERSECTION_STATE_NSS_R";

    case INTERSECTION_STATE_NST_G:
      return "INTERSECTION_STATE_NST_G";
    case INTERSECTION_STATE_NST_Y:
      return "INTERSECTION_STATE_NST_Y";
    case INTERSECTION_STATE_NST_R:
      return "INTERSECTION_STATE_NST_R";

    case INTERSECTION_STATE_EWS_G:
      return "INTERSECTION_STATE_EWS_G";
    case INTERSECTION_STATE_EWS_Y:
      return "INTERSECTION_STATE_EWS_Y";
    case INTERSECTION_STATE_EWS_R:
      return "INTERSECTION_STATE_EWS_R";

    case INTERSECTION_STATE_EWT_G:
      return "INTERSECTION_STATE_EWT_G";
    case INTERSECTION_STATE_EWT_Y:
      return "INTERSECTION_STATE_EWT_Y";
    case INTERSECTION_STATE_EWT_R:
      return "INTERSECTION_STATE_EWT_R";
  }
  return NULL;
}

/*the possible states for a single light*/
enum traffic_light_state{
  LIGHT_GREEN,
  LIGHT_YELLOW,
  LIGHT_RED
};

/*a structure holding the three possible lights per road*/
struct road_lights{
  enum traffic_light_state left;
  enum traffic_light_state straight;
  enum traffic_light_state right;
};

/*the structure holding all of the intersection state*/
struct intersection{

  /*the state machine state of the intersection*/
  enum intersection_state state;

  /*the state of the individual traffic lights of the intersection*/
  struct road_lights road_north;
  struct road_lights road_east;
  struct road_lights road_south;
  struct road_lights road_west;
};

static inline void intersection_init(struct intersection *i){

  /*setting the initial state*/
  i->state = INTERSECTION_STATE_NSS_G;

  /*setting the initial state of the traffic lights (all lights to RED)*/
  i->road_north.left     = LIGHT_RED;
  i->road_north.straight = LIGHT_RED;
  i->road_north.right    = LIGHT_RED;

  i->road_east.left      = LIGHT_RED;
  i->road_east.straight  = LIGHT_RED;
  i->road_east.right     = LIGHT_RED;

  i->road_south.left     = LIGHT_RED;
  i->road_south.straight = LIGHT_RED;
  i->road_south.right    = LIGHT_RED;

  i->road_west.left      = LIGHT_RED;
  i->road_west.straight  = LIGHT_RED;
  i->road_west.right     = LIGHT_RED;
}

#endif
