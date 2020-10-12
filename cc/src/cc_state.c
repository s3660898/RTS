#include <stdio.h>
#include <stdlib.h>

#include "cc_state.h"

void cc_state_init(struct cc_state *ccs){
  pthread_mutex_init(&ccs->mutex, NULL);

  ccs->i1_time = 0;
  intersection_init(&ccs->i1);

  ccs->i2_time = 0;
  intersection_init(&ccs->i2);

  ccs->xc_time = 0;
  ccs->xc.state = TRAIN_CROSSING_GREEN;

  ccs->p1_time = 0;
  pedestrian_crossings_init(&ccs->p1);

  ccs->p2_time = 0;
  pedestrian_crossings_init(&ccs->p2);
}
