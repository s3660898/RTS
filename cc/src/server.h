#ifndef SERVER_H
#define SERVER_H

struct server{
  int pid;
  int chid;
};

/*returns 1 on failure, 0 on success*/
int server_init(struct server *s);

/*closes the channel for the open server*/
int server_destroy(struct server *s);

#endif
