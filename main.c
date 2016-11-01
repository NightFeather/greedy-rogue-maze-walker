#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //usleeep, random, srandom
#include "maze.h"
#include "path.h"

int main(void){
  Maze* maze;
  Path* path;
  int status = 0;
  int rb_status = 0;
  maze = create_map(25,25);
  path = init_path(maze);
  printf("size: %d, %d\n", maze->xsize, maze->ysize);
  while(1){
    show_map(maze,2);
    printf("current: %d, %d\n", path->current->pos.x,path->current->pos.y);
    status = next(path);
    if(status == 1) break; // done!
    if(status == -1) rb_status = rollback(path);
    if(rb_status == -1) break;
    usleep(100000);
    printf("\e[H\e[2J");
  }
  printf("\e[H\e[2J");
  show_map(maze,2);
  if(rb_status == -1) printf("No route to goal.\n");
  if(status == 1) printf("Goal!!\n");
  delete_map(maze);
  destroy_path(path);
  free(maze);
  return 0;
}
