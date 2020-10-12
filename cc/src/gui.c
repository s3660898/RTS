#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>

#include "pedestrian.h"

#include "gui.h"

void gui_init(void){
  initscr();
  cbreak();
  curs_set(0);

  /*initiailising terminal colours*/
  if(has_colors() == FALSE)
    puts("terminal does not support colour");
  else
    start_color();

  /*definining colour pairs for traffic lights*/
  init_pair(1, COLOR_GREEN, COLOR_BLACK);
  init_pair(2, COLOR_YELLOW, COLOR_BLACK);
  init_pair(3, COLOR_RED, COLOR_BLACK);
}

void gui_draw_light(unsigned y, unsigned x, enum traffic_light_state tls, const char *direction){

  switch(tls){
    case LIGHT_GREEN:
      attron(COLOR_PAIR(1));
      mvprintw(y, x, "%10s", direction);
      attroff(COLOR_PAIR(1));
      break;
    case LIGHT_YELLOW:
      attron(COLOR_PAIR(2));
      mvprintw(y, x, "%10s", direction);
      attroff(COLOR_PAIR(2));
      break;
    case LIGHT_RED:
      attron(COLOR_PAIR(3));
      mvprintw(y, x, "%10s", direction);
      attroff(COLOR_PAIR(3));
      break;
  }

}

void gui_draw_lights(unsigned y, unsigned x, struct road_lights *rl, const char *direction){

  /*printing string*/
  mvprintw(
    y, x,
    "%4s  %25s:",
    "    ",
    direction
  );

  /*printing light states*/
  gui_draw_light(y, x+35, rl->left, "LEFT");
  gui_draw_light(y, x+45, rl->straight, "STRAIGHT");
  gui_draw_light(y, x+55, rl->right, "RIGHT");
}

void gui_draw_intersection(unsigned y, unsigned x, struct intersection *isection, const char *name, clock_t time){

  float time_since_message = (float)(clock() - time)/CLOCKS_PER_SEC;

  /*printing the time since the last message*/
  if(time_since_message >= 5.0f)
    attron(COLOR_PAIR(3));

  mvprintw(
    y, x,
    "%4s, %25s: %3.2f",
    name,
    "secs since last message",
    time_since_message
  );

  if(time_since_message >= 5.0f)
    attroff(COLOR_PAIR(3));

  /*printing the current state*/
  mvprintw(
    y+1, x+0,
    "%5s %25s: %s",
    "",
    "intersection state",
    intersection_state_string(isection->state)
  );

  /*printing the traffic light's state*/
  gui_draw_lights(y+2, x+0, &isection->road_north, "north road");
  gui_draw_lights(y+3, x+0, &isection->road_east, "east road");
  gui_draw_lights(y+4, x+0, &isection->road_south, "south road");
  gui_draw_lights(y+5, x+0, &isection->road_west, "west road");

};

void gui_draw_crossing(int y, int x, enum pedestrian_crossing_state pcs, const char *name){
  switch(pcs){
    case PEDESTRIAN_CROSSING_RED:
      attron(COLOR_PAIR(3));
      mvprintw(y, x, "%10s", name);
      attroff(COLOR_PAIR(3));
      break;

    case PEDESTRIAN_CROSSING_GREEN:
      attron(COLOR_PAIR(1));
      mvprintw(y, x, "%10s", name);
      attroff(COLOR_PAIR(1));
      break;
  }
}

void gui_draw_train_crossing(int y, int x, struct train_crossing *tc, const char *name, clock_t time){

  float time_since_message = (float)(clock() - time)/CLOCKS_PER_SEC;

  mvprintw(
    y+0, x+0,
    "%4s, %25s: %3.2f",
    name,
    "secs since last message",
    time_since_message
  );

  mvprintw(
    y+1, x+0,
    "%5s %25s:",
    "",
    "crossing state"
  );

  switch(tc->state){
    case TRAIN_CROSSING_GREEN:
      attron(COLOR_PAIR(3));
      mvprintw(y+1, x+35, "%10s", "VEHICLES");
      attroff(COLOR_PAIR(3));
      break;

    case TRAIN_CROSSING_RED:
      attron(COLOR_PAIR(1));
      mvprintw(y+1, x+35, "%10s", "VEHICLES");
      attroff(COLOR_PAIR(1));
      break;
  }
}

void gui_draw_pedestrian_crossings(int y, int x, struct pedestrian_crossings *p, const char *name, clock_t time){

  float time_since_message = (float)(clock() - time)/CLOCKS_PER_SEC;

  mvprintw(
    y+0, x+0,
    "%4s, %25s: %3.2f",
    name,
    "secs since last message",
    time_since_message
  );

  mvprintw(
    y+1, x+0,
    "%5s %25s:",
    "",
    "crossing states"
  );

  gui_draw_crossing(y+1, x+35, p->north, "NORTH");
  gui_draw_crossing(y+1, x+45, p->south, "SOUTH");
  gui_draw_crossing(y+1, x+55, p->east, "EAST");
  gui_draw_crossing(y+1, x+65, p->west, "WEST");

}

void gui_solve(struct cc_state *ccs){

  erase();

  pthread_mutex_lock(&ccs->mutex);

  gui_draw_intersection(1, 0, &ccs->i1, "IC1", ccs->i1_time);
  gui_draw_pedestrian_crossings(8, 0, &ccs->p1, "PC1", ccs->p1_time);
  gui_draw_train_crossing(11, 0, &ccs->xc, "XC", ccs->xc_time);
  gui_draw_intersection(14, 0, &ccs->i2, "IC2", ccs->i2_time);
  gui_draw_pedestrian_crossings(21, 0, &ccs->p2, "PC2", ccs->p2_time);

  pthread_mutex_unlock(&ccs->mutex);

  /*drawing to the screen*/
  refresh();
}
