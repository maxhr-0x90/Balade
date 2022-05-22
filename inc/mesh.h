/* Module gérant les meshs */

#ifndef MESH_H_
#define MESH_H_

#include "vector.h"
#include "material.h"
#include "gen_array.h"

// Structure des coordonnées 3D
struct coord3d_s {
  float x, y, z;
};

// Structure des coordonnées 2D
struct coord2d_s {
  float x, y;
};

typedef struct coord3d_s *vertex;
typedef struct coord3d_s *normal;
typedef struct coord2d_s *uvcoord;

// Structure d'une face
typedef struct face_s *face;
struct face_s {
  int vertices[3];
  int normal;
  int uvcoord[3];
};

// Structure d'un mesh
typedef struct mesh_s* mesh;
struct mesh_s {
  material material;
  array vertices;
  array normals;
  array uvcoords;
  array faces;
};


// Initialise un mesh
mesh mesh_init(material mat, int nb_face, int nb_vert, int nb_norm, int nb_uvc);

// Libère l'espace occupé par un mesh
void mesh_free(mesh m);

// Ajoute un point au mesh
void mesh_add_vert(vertex v, mesh m);

// Ajoute une normale au mesh
void mesh_add_norm(normal n, mesh m);

// Ajoute une coordonnée uv au mesh
void mesh_add_uvc(uvcoord u, mesh m);

// Ajoute une face au mesh
void mesh_add_face(face f, mesh m);

// Initialise un point
vertex vertex_init(float x, float y, float z);

// Initialise une normale
normal normal_init(float x, float y, float z);

// Initialise une coordonnée uv
uvcoord uvcoord_init(float x, float y);

// Initialise une face
face face_init(int v0, int v1, int v2, int u0, int u1, int u2, int norm);

// Copie un mesh
void *mesh_copy(void *src_ptr);

#endif