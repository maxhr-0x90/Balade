/* Module de calcul des collisions */

#ifndef COLLISION_H_
#define COLLISION_H_

#include "vector.h"

// AABB : Axis Aligned Bounding Box 
// OBB  : Oriented Bounding Box

// Retourne 1 s'il y a une superposition entre une AABB (3D) et un triangle, 0 sinon
int triangle_AABB_overlap(
  vector3f t0, vector3f t1, vector3f t2,
  vector3f c000, vector3f c111
);

// Retourne 1 s'il y a une superposition entre un cercle et un cercle, 0 sinon
int circle_circle_overlap(
  vector2f c1, float r1,
  vector2f c2, float r2
);

// Retourne 1 si cercle in est dans cercle out, 0 sinon
int circle_circle_inside(
  vector2f cin, float rin,
  vector2f cout, float rout
);

// Retourne 1 s'il y a une superposition entre une AABB (2D) et un cercle, 0 sinon
int circle_aabb_overlap(
  vector2f c, float r,
  vector2f mid, vector2f h
);

// Retourne 1 si le cercle se trouve dans une AABB (2D), 0 sinon
int circle_aabb_inside(
  vector2f c, float r,
  vector2f mid, vector2f h
);

// Retourne 1 s'il y a une superposition entre 2 AABB (2D), 0 sinon
int aabb_aabb_overlap(
  vector2f mid1, vector2f h1,
  vector2f mid2, vector2f h2
);

// Retourne 1 s'il y a une superpositon entre une AABB (2D) et une OBB (2D), 0 sinon
int obb_aabb_overlap(
  vector2f c, vector2f hx, vector2f hy,
  vector2f mid, vector2f h
);

#endif