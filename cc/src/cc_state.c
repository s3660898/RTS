#include <stdio.h>
#include <stdlib.h>

#include "cc_state.h"

void intersection_shared_init(struct intersection_shared *is){
  pthread_mutex_init(&is->mutex, NULL);
  intersection_init(&is->i);
}

void cc_state_init(struct cc_state *ccs){
  intersection_shared_init(&ccs->i1);
  intersection_shared_init(&ccs->i2);
}
