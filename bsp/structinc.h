#pragma once
// structs NOTE remember to instantiate struct objs in init func
typedef struct {
    float x, y;
    int z, a, l;
}player;

typedef struct {
    float SIN[360], COS[360];
}trig;

typedef struct {
    int x1, x2, y1, y2, z;
}sectors;