#include <string.h>

#include "../inc/mesh.h"
#include "../inc/alloc.h"

mesh mesh_init(material mat, int nb_face, int nb_vert, int nb_norm, int nb_uvc){
  mesh m = safe_alloc(sizeof(struct mesh_s));
  m->material = mat;
  m->faces = array_init(nb_face);
  m->vertices = array_init(nb_vert);
  m->normals = array_init(nb_norm);
  m->uvcoords = array_init(nb_uvc);

  return m;
}

void mesh_free(mesh m){
  if (m == NULL){ return; }

  array_free(1, m->faces);
  array_free(1, m->normals);
  array_free(1, m->vertices);
  array_free(1, m->uvcoords);
  safe_free(m);
}

void mesh_add_vert(vertex v, mesh m){
  if (m == NULL){ return; }

  array_add(v, m->vertices);
}

void mesh_add_norm(normal n, mesh m){
  if (m == NULL){ return; }

  array_add(n, m->normals);
}

void mesh_add_uvc(uvcoord u, mesh m){
  if (m == NULL){ return; }

  array_add(u, m->uvcoords);
}

void mesh_add_face(face f, mesh m){
  if (m == NULL){ return; }

  array_add(f, m->faces);
}

vertex vertex_init(float x, float y, float z){
  vertex v = safe_alloc(sizeof(vector3f));
  (*v)[0] = x;
  (*v)[1] = y;
  (*v)[2] = z;

  return v;
}

normal normal_init(float x, float y, float z){
  normal n = safe_alloc(sizeof(vector3f));
  (*n)[0] = x;
  (*n)[1] = y;
  (*n)[2] = z;

  return n;
}

uvcoord uvcoord_init(float x, float y){
  uvcoord u = safe_alloc(sizeof(vector2f));
  (*u)[0] = x;
  (*u)[1] = y;

  return u;
}

face face_init(int v0, int v1, int v2, int u0, int u1, int u2, int norm){
  face f = safe_alloc(sizeof(struct face_s));
  f->vertices[0] = v0;
  f->vertices[1] = v1;
  f->vertices[2] = v2;
  f->uvcoord[0] = u0;
  f->uvcoord[1] = u1;
  f->uvcoord[2] = u2;
  f->normal = norm;

  return f;
}

void *vertex_cpy(void *ptr){
  if (ptr == NULL){ return NULL; }

  vertex v = (vertex)ptr;
  vertex c = safe_alloc(sizeof(vector3f));
  memcpy(c, v, sizeof(vector3f));

  return c;
}

void *normal_cpy(void *ptr){
  if (ptr == NULL){ return NULL; }

  normal n = (normal)ptr;
  normal c = safe_alloc(sizeof(vector3f));
  memcpy(c, n, sizeof(vector3f));

  return c;
}

void *uvcoord_cpy(void *ptr){
  if (ptr == NULL){ return NULL; }

  uvcoord u = (uvcoord)ptr;
  uvcoord c = safe_alloc(sizeof(vector2f));
  memcpy(c, u, sizeof(vector2f));

  return c;
}

void *face_cpy(void *ptr){
  if (ptr == NULL){ return NULL; }

  face f = (face)ptr;
  face c = safe_alloc(sizeof(struct face_s));
  memcpy(c, f, sizeof(struct face_s));

  return c;
}

void *mesh_copy(void *src_ptr){
  if (src_ptr == NULL){ return NULL; }

  mesh src = (mesh)src_ptr;

  mesh cpy = safe_alloc(sizeof(struct mesh_s));
  cpy->material = src->material;
  cpy->vertices = array_copy(vertex_cpy, src->vertices);
  cpy->normals = array_copy(normal_cpy, src->normals);
  cpy->uvcoords = array_copy(uvcoord_cpy, src->uvcoords);
  cpy->faces = array_copy(face_cpy, src->faces);

  return cpy;
}