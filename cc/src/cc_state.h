#ifndef CC_STATE_H
#define CC_STATE_H

#include <pthread.h>

#include "intersection.h"

/*a thread safe intersection state struct*/
struct intersection_shared{
  struct intersection i;
  pthread_mutex_t mutex;
};

void intersection_shared_init(struct intersection_shared *is);

/*the main struct containing state for the CC*/
struct cc_state{
  struct intersection_shared i1;
  struct intersection_shared i2;
};

void cc_state_init(struct cc_state *ccs);

#endif
