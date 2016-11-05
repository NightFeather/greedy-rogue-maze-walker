#include <stdio.h>
#include <stdlib.h>
#include "maze.h"
#include "path.h"

Path* init_path(Maze* maze){

  Path*     path;
  PathNode* start;
  Coord     pos;

  path  = (Path*) malloc(sizeof(Path));
  start = (PathNode*) malloc(sizeof(PathNode));

  start->prev = NULL;
  pos.x = 1;
  pos.y = 1;
  start->pos = pos;

  maze->map[1][1].state = 1;
  path->maze    = maze;
  path->start   = start;
  path->current = start;
  path->length  = 0;

  return path;

}

int move(Path* path, int priority) {

  Maze* maze = path->maze;
  MazeUnit *maze_unit, *maze_unit_nxt;
  Coord pos = path->current->pos;
  Coord next_pos;

/*
 *  when pos.x > pos.y
 *  7 6 4
 *  5   2
 *  3 1 0
 *
 *  when pos.x < pos.y
 *  7 5 3
 *  6   1
 *  4 2 0
 */

  int remap_ur[8] = {5,2,1,6,4,3,0,7};
  int remap_dl[8] = {5,1,2,4,6,0,3,7};
  int direction = -1;

  maze_unit = &maze->map[pos.x][pos.y];

  if(pos.x > pos.y){
    direction = remap_ur[priority];
  } else {
    direction = remap_dl[priority];
  }

/*
 * direction:
 *  0: up
 *  1: right
 *  2: down
 *  3: left
 *  4: up right
 *  5: down right
 *  6: down left
 *  7: up left
 */

  switch(direction){
    case 0:
      next_pos.x = pos.x;
      next_pos.y = pos.y-1;
      break;
    case 1:
      next_pos.x = pos.x+1;
      next_pos.y = pos.y;
      break;
    case 2:
      next_pos.x = pos.x;
      next_pos.y = pos.y+1;
      break;
    case 3:
      next_pos.x = pos.x-1;
      next_pos.y = pos.y;
      break;
    case 4:
      if(maze_unit->mode != 2){return -1;}
      next_pos.x = pos.x+1;
      next_pos.y = pos.y-1;
      break;
    case 5:
      if(maze_unit->mode != 2){return -1;}
      next_pos.x = pos.x+1;
      next_pos.y = pos.y+1;
      break;
    case 6:
      if(maze_unit->mode != 2){return -1;}
      next_pos.x = pos.x-1;
      next_pos.y = pos.y+1;
      break;
    case 7:
      if(maze_unit->mode != 2){return -1;}
      next_pos.x = pos.x-1;
      next_pos.y = pos.y-1;
      break;
    default:
      return -1;
  }

  // takeout the info to shorten the next if expression
  maze_unit_nxt = &maze->map[next_pos.x][next_pos.y];

  if(getenv("DEBUG") != NULL) {
    printf("%d: try move from (%d, %d) to (%d, %d)\n", direction, pos.x, pos.y, next_pos.x, next_pos.y);
  }

  if(maze_unit_nxt->mode != 1 && maze_unit_nxt->state == 0){
    PathNode* node;
    node = (PathNode*) malloc(sizeof(PathNode));
    node->pos = next_pos;
    node->prev = path->current;
    path->current = node;
    maze_unit->state = 1;
    maze_unit_nxt->state = 3;
    path->length += 1;
    return 0;
  } else {
    return -1;
  }

}

int next(Path* path) {
  int i =0;
  int status = 0;
  PathNode min;

  for( i=0; i < 8; ++i){
    status = move(path, i);
    if(status != -1) break;
  }

  if(path->current->pos.x == path->maze->xsize-2 &&
    path->current->pos.y == path->maze->ysize-2 ) return 1;
  if(status == -1 && i == 8) return -1; // seems no way to go
  return 0;
}

int rollback(Path* path){
  PathNode* temp;
  if(path->current == path->start) return -1; //back to start, no way
  temp = path->current;
  path->current = path->current->prev;

  if(getenv("DEBUG") != NULL) {
      printf("rollback from (%d, %d) to (%d, %d).\n",
            temp->pos.x, temp->pos.y,
            path->current->pos.x, path->current->pos.y );
  }

  path->maze->map[temp->pos.x][temp->pos.y].state = 2;
  path->length -= 1;
  free(temp);
  return 0;
}

void  destroy_path(Path* path){
  PathNode* temp;
  int i = 0;

  while(path->current->prev != NULL){
    temp = path->current;
    path->current = path->current->prev;
    free(temp);
    i += 1;
  }

  free(path->start);

  printf("%d nodes freed.\n", i+1);

  free(path);
}