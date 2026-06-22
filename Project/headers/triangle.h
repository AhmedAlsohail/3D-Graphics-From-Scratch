#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "vector.h"
// stores vertex index
typedef struct
{
    int a;
    int b;
    int c;
} face_t;

// stores vertex actual vector2 points
typedef struct
{
    vector2 points[3];
} triangle_t;
#endif