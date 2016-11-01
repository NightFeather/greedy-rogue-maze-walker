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

  return path;

}

int move(Path* path, int direction) {
  Maze* maze = path->maze;
  MazeUnit *maze_unit, *maze_unit_nxt;
  Coord pos = path->current->pos;
  Coord next_pos;

/*
 *764
 *5 2
 *310
 */

  maze_unit = &maze->map[pos.x][pos.y];
  switch(direction){ // 0: down_right, 1: down, 2: right, 3: down_left, 4: right_up, 5: left, 6: up, 7: up_left
    case 0:
      if(maze_unit->mode != 2){return -1;}
      next_pos.x = pos.x+1;
      next_pos.y = pos.y+1;
      break;
    case 1:
      next_pos.x = pos.x;
      next_pos.y = pos.y+1;
      break;
    case 2:
      next_pos.x = pos.x+1;
      next_pos.y = pos.y;
      break;
    case 3:
      if(maze_unit->mode != 2){return -1;}
      next_pos.x = pos.x-1;
      next_pos.y = pos.y+1;
      break;
    case 4:
      if(maze_unit->mode != 2){return -1;}
      next_pos.x = pos.x+1;
      next_pos.y = pos.y-1;
      break;
    case 5:
      next_pos.x = pos.x-1;
      next_pos.y = pos.y;
      break;
    case 6:
      next_pos.x = pos.x;
      next_pos.y = pos.y-1;
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

  printf("%d: try move from (%d, %d) to (%d, %d)\n", direction, pos.x, pos.y, next_pos.x, next_pos.y);

  if(maze_unit_nxt->mode != 1 && maze_unit_nxt->state == 0){
    PathNode* node;
    node = (PathNode*) malloc(sizeof(PathNode));
    node->pos = next_pos;
    node->prev = path->current;
    path->current = node;
    maze_unit->state = 1;
    maze_unit_nxt->state = 3;
    return 0;
  } else {
    return -1;
  }

}

int next(Path* path) {
  int i =0;
  int status = 0;
  while(((status = move(path, i)) == -1) && (i < 8) ){ ++i; }
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
  printf("rollback from (%d, %d) to (%d, %d).\n",
          temp->pos.x, temp->pos.y,
          path->current->pos.x, path->current->pos.y );
  path->maze->map[temp->pos.x][temp->pos.y].state = 2;
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