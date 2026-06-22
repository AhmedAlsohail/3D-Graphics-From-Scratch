#ifndef MESHES_H
#define MESHES_H

#include "vector.h"
#include "triangle.h"

#define N_MESH_VERTICES 8
extern vector3 cube_vertices[N_MESH_VERTICES];

#define N_MESH_FACES (6 * 2) // 6 Cube faces, 2 triangles each.
extern face_t cube_faces[N_MESH_FACES];

#endif