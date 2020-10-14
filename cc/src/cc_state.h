#ifndef CC_STATE_H
#define CC_STATE_H

#include <pthread.h>

#include "intersection.h"
#include "pedestrian.h"
#include "train.h"
#include "mode.h"
#include "clients.h"

struct client_info;

/*the main struct containing state for the CC*/
struct cc_state{

  /*intersection i1 related*/
  clock_t i1_time;                 /*time since last message received*/
  struct intersection i1;          /*contains the state of the intersection*/

  clock_t p1_time;
  struct pedestrian_crossings p1;  /*contains the state of the pedestrian crossing*/

  clock_t x_time;
  struct train_crossing x;

  /*intersection i2 related*/
  clock_t i2_time;
  struct intersection i2;

  clock_t p2_time;
  struct pedestrian_crossings p2;  /*contains the state of the pedestrian crossing*/

  /*the operating mode of the system*/
  enum mode mode;

  /*state multithreading related*/
  pthread_mutex_t mutex;

  /*connection information for where cc is client*/
  struct client_info ci_ic1;
  struct client_info ci_ic2;
};

void cc_state_init(struct cc_state *ccs);

#endif
