#ifndef CC_STATE_H
#define CC_STATE_H

#include <pthread.h>

#include "intersection.h"
#include "pedestrian.h"
#include "train.h"

/*a thread safe intersection state struct*/

/*the main struct containing state for the CC*/
struct cc_state{

  /*intersection i1 related*/
  clock_t i1_time;                 /*time since last message received*/
  struct intersection i1;          /*contains the state of the intersection*/

  clock_t p1_time;
  struct pedestrian_crossings p1;  /*contains the state of the pedestrian crossing*/

  clock_t xc_time;
  struct train_crossing xc;

  /*intersection i2 related*/
  clock_t i2_time;
  struct intersection i2;

  clock_t p2_time;
  struct pedestrian_crossings p2;  /*contains the state of the pedestrian crossing*/

  /*multithreading related*/
  pthread_mutex_t mutex;
};

void cc_state_init(struct cc_state *ccs);

#endif
