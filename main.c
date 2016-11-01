#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "maze.h"
#include "path.h"

int main(void){
  Maze* maze;
  Path* path;
  int status = 0;
  int rb_status = 0;
  maze = create_map(15,15);
  path = init_path(maze);
  printf("size: %d, %d\n", maze->xsize, maze->ysize);
  while(1){
    show_map(maze,2);
    printf("current: %d, %d\n", path->current->pos.x,path->current->pos.y);
    status = next(path);
    if(status == 1) break; // done!
    if(status == -1) rb_status = rollback(path);
    if(rb_status == -1) break;
  }
  show_map(maze,2);
  delete_map(maze);
  destroy_path(path);
  free(maze);
  return 0;
}
