#ifndef _MAZE_H
  #define _MAZE_H

  void rand_setup();

  struct _maze_unit {
    int mode;
    int state;
  };
  
  struct _maze {
    int                 xsize ;
    int                 ysize ;
    struct _maze_unit** map   ;
  };
  
  typedef struct _maze_unit MazeUnit;
  typedef struct _maze      Maze;
  
  Maze* create_map( int, int );
  int   show_map( Maze*, int);
  int   delete_map( Maze* );
#endif
