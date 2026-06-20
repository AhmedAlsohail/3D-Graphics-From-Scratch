#include <math.h>
#include "vector.h"

// TODO: Implement vectors manipulation functions
vector3 rotateVectorX(vector3 rotation, float angle){
    vector3 rotatedVector = {
        rotation.x,
        rotation.y * cos(angle) - rotation.z * sin(angle),
        rotation.y * sin(angle) + rotation.z * cos(angle),
    };

    return rotatedVector;
}

vector3 rotateVectorY(vector3 rotation, float angleX, float angleY){
    vector3 rotatedVectorY = {
        rotation.x * cos(angleY) - rotation.z * sin(angleY),
        rotation.y,
        rotation.x * sin(angleY) + rotation.z * cos(angleY),
    };

    vector3 rotatedVectorX = {
        rotatedVectorY.x,
        rotatedVectorY.y * cos(angleX) - rotatedVectorY.z * sin(angleX),
        rotatedVectorY.y * sin(angleX) + rotatedVectorY.z * cos(angleX),
    };

    return rotatedVectorX;
}

vector3 rotateVectorZ(vector3 rotation, float angle){
    vector3 rotatedVector = {
        rotation.x * cos(angle) - rotation.y * sin(angle),
        rotation.x * sin(angle) + rotation.y * cos(angle),
        rotation.z,
    };

    return rotatedVector;
}