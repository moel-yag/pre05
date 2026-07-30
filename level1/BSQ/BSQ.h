#ifndef BSQ_H
#define BSQ_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_point
{
    int x;
    int y;
} t_point;

typedef struct s_game
{
    int width;
    int height;
    char empty;
    char obstacle;
    char draw;
    char **cells;
    t_point start;
    int square_size;
} t_game;

#endif