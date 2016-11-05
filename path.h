#ifndef _PATH_H_
#define _PATH_H_

struct _coord
{
  int x;
  int y;  
};

struct _path_node
{
  struct _path_node* prev;

  struct _coord pos;

};

struct _path
{
  Maze* maze;
  int length;

  struct _path_node* start;
  struct _path_node* current;
  struct _path_node* nearest;
};

typedef struct _path       Path;
typedef struct _path_node  PathNode;
typedef struct _coord      Coord;

Path*    init_path(Maze*);
int       move(Path*,int);
int           next(Path*);
int       rollback(Path*);
void  destroy_path(Path*);

#endif
