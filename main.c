#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //usleeep, random, srandom
#include "maze.h"
#include "path.h"

int main(int argc, char* argv[]){

  if(argc < 3) {
    printf("Usage: %s width height \n", argv[0]);
    return -1;
  }

  int xsize = atoi(argv[1]);
  int ysize = atoi(argv[2]);

  if(xsize < 3 || ysize < 3){
    printf("Cannot build maze with any side less than 3!\n");
    return -2;
  }

  Maze* maze;
  Path* path;
  int status = 0;
  int rb_status = 0;
  maze = create_map(xsize,ysize);
  path = init_path(maze);
  printf("size: %d, %d\n", maze->xsize, maze->ysize);
  while(1){

    if(getenv("DEBUG") != NULL) {
      show_map(maze,2);
      printf("current: %d, %d\n", path->current->pos.x,path->current->pos.y);
      printf("length: %d steps\n", path->length);
    }

    status = next(path);
    if(status == 1) break; // done!
    if(status == -1) rb_status = rollback(path);
    if(rb_status == -1) break;

    if(getenv("DEBUG") != NULL) {
      usleep(100000);
      printf("\e[H\e[2J");
    }
  }
  printf("\e[H\e[2J");
  show_map(maze,2);
  printf("length: %d\n", path->length);
  if(rb_status == -1) printf("No route to goal.\n");
  if(status == 1) printf("Goal!!\n");
  delete_map(maze);
  destroy_path(path);
  free(maze);
  return 0;
}
