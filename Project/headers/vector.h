#ifndef VECTOR_H
#define VECTOR_H

typedef struct{
    float x;
    float y;
} vector2;

typedef struct{
    float x;
    float y;
    float z;
} vector3;


// TODO: Add vectors manipulation function
vector3 rotateVectorX(vector3 rotation, float angle);
vector3 rotateVectorY(vector3 rotation, float angleX, float angleY);
vector3 rotateVectorZ(vector3 rotation, float angle);
#endif