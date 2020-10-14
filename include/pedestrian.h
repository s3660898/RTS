#ifndef PEDESTRIAN_H
#define PEDESTRIAN_H

enum pedestrian_crossing_state{
  PEDESTRIAN_CROSSING_RED,
  PEDESTRIAN_CROSSING_YELLOW,
  PEDESTRIAN_CROSSING_GREEN
};

struct pedestrian_crossings{
  enum pedestrian_crossing_state north;
  enum pedestrian_crossing_state east;
  enum pedestrian_crossing_state south;
  enum pedestrian_crossing_state west;
};

static inline void pedestrian_crossings_init(struct pedestrian_crossings *p){
  p->north = PEDESTRIAN_CROSSING_RED;
  p->east  = PEDESTRIAN_CROSSING_RED;
  p->south = PEDESTRIAN_CROSSING_RED;
  p->west  = PEDESTRIAN_CROSSING_RED;
}

#endif
