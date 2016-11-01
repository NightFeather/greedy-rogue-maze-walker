#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "maze.h"

void rand_setup(){
  struct timespec ts;
  timespec_get(&ts, TIME_UTC);
  srandom(ts.tv_nsec ^ ts.tv_sec);
}

Maze* create_map( int xsize, int ysize ){
  if(xsize < 1 || ysize < 1) return NULL; //validates the size

  rand_setup();

  Maze * base;
  base = (Maze*) malloc(sizeof(Maze));
  base->xsize = xsize;
  base->ysize = ysize;

  MazeUnit ** temp;
  temp = (MazeUnit**) calloc(base->xsize, sizeof(MazeUnit*));

  for(int i = 0; i < base->xsize; i++ ){
    temp[i] = (MazeUnit*)calloc(base->ysize, sizeof(MazeUnit));
    for(int j = 0; j< base->ysize; j++ ){
      if( i == 0 ||
          j == 0 ||
          i == (base->xsize-1) ||
          (j == base->ysize-1)){
        temp[i][j].mode  = 1; // wall
      } else {
        temp[i][j].mode  = abs(random()%3);
      }
      temp[i][j].state = 0;
    }
  }

  temp[1][1].mode = 2; // make sure the start point walkable
  temp[base->xsize-2][base->ysize-2].mode = 2;

  base->map = temp;
  return base;
}

int show_map( Maze* base, int type){
  if(base->map == NULL) return 1;
  for(int i = 0; i < base->xsize;putchar('\n'), ++i) {
    for (int j = 0; j < base->ysize; putchar(' '), ++j) {
      MazeUnit node = base->map[i][j];
      if(node.state == 1) printf("\e[1;33m");
      if(node.state == 2) printf("\e[1;31m");
      if(node.state == 3) printf("\e[1;35m");
      switch(type){
        case 0:
          printf("%d", node.mode);
          break;
        case 1:
          printf("%d", node.state);
          break;
        case 2:
        default:
          printf("%d(%d)", node.mode, node.state);
      }
      printf("\e[m");
    }
  }
}

int delete_map( Maze* base ){
  if(base->map == NULL) return 1;
  for(int i = 0; i < base->xsize; i ++){
    free(base->map[i]);
  }
  free(base->map);
  base->map = NULL;
  return 0;
}
