#include <utility>
#pragma once
// structs NOTE remember to instantiate struct objs in init func
typedef struct {
    float x, y;
    int z, a, l;
}player;

typedef struct {
    float SIN[360], COS[360];
}trig;

typedef struct Walls {
    std::pair<float, float> p1;
    std::pair<float, float> p2;
    std::pair<int, int> normal;
    int zdep;
    int col;

    Walls(int x1, int y1, int x2, int y2) : p1(x1, y1), p2(x2, y2) {};
    Walls(int x1, int y1, int x2, int y2, int col_) : p1(x1, y1), p2(x2, y2), col(col_) {};
}Walls;

typedef struct {
    int ws, we; //wall start and end indices
    int zfloor, zheight;
    int x, y; //center coord
    int d;
}sectors;