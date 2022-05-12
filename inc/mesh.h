#ifndef MESH_H_
#define MESH_H_

#include "vector.h"
#include "material.h"
#include "gen_array.h"

typedef vector3f *vertex;
typedef vector3f *normal;
typedef vector2f *uvcoord;

typedef struct face_s *face;
struct face_s {
  int vertices[3];
  int normal;
  int uvcoord[3];
};

typedef struct mesh_s* mesh;
struct mesh_s {
  material material;
  array vertices;
  array normals;
  array uvcoords;
  array faces;
};

mesh mesh_init(material mat, int nb_face, int nb_vert, int nb_norm, int nb_uvc);

void mesh_free(mesh m);

void mesh_add_vert(vertex v, mesh m);

void mesh_add_norm(normal n, mesh m);

void mesh_add_uvc(uvcoord u, mesh m);

void mesh_add_face(face f, mesh m);

vertex vertex_init(float x, float y, float z);

normal normal_init(float x, float y, float z);

uvcoord uvcoord_init(float x, float y);

face face_init(int v0, int v1, int v2, int u0, int u1, int u2, int norm);

void *mesh_copy(void *src_ptr);

#endif