#ifndef MODE_H
#define MODE_H

enum mode{
  MODE_FIXED,
  MODE_SENSOR,
  MODE_MANUAL
};

static inline const char *mode_string(enum mode m){
  switch(m){
    case MODE_FIXED:
      return "MODE_FIXED";
    case MODE_SENSOR:
      return "MODE_SENSOR";
    case MODE_MANUAL:
      return "MODE_MANUAL";
  }

  return NULL;
}

#endif
