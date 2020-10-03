#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

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

void gui_draw_intersection(unsigned y, unsigned x, struct intersection *i){
};

void gui_solve(struct cc_state *ccs){

  /*printing the current state*/
  mvprintw(
    0, 0,
    "%4s: %20s: %s",
    "IC1",
    "intersection state",
    intersection_state_string(ccs->i1.i.state)
  );

  /*printing the traffic light's state*/
  mvprintw(
    1, 0,
    "%4s: %20s:",
    "    ",
    "north road lights"
  );

  attron(COLOR_PAIR(1));
  mvaddstr(1, 30, "LEFT");
  mvaddstr(1, 40, "STRAIGHT");
  mvaddstr(1, 50, "RIGHT");
  attroff(COLOR_PAIR(1));

  /*drawing to the screen*/
  refresh();
}
