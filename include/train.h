#ifndef TRAIN_H
#define TRAIN_H

enum train_crossing_state{
  TRAIN_CROSSING_RED,
  TRAIN_CROSSING_GREEN
};

struct train_crossing{
  enum train_crossing_state state;
};

#endif
