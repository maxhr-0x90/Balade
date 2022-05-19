#ifndef COLLISION_H_
#define COLLISION_H_

#include "vector.h"

int triangle_AABB_overlap(
  vector3f t0, vector3f t1, vector3f t2,
  vector3f c000, vector3f c111
);

int circle_circle_overlap(
  vector2f c1, float r1,
  vector2f c2, float r2
);

int circle_circle_inside(
  vector2f cin, float rin,
  vector2f cout, float rout
);

int circle_aabb_overlap(
  vector2f c, float r,
  vector2f mid, vector2f h
);

int circle_aabb_inside(
  vector2f c, float r,
  vector2f mid, vector2f h
);

int aabb_aabb_overlap(
  vector2f mid1, vector2f h1,
  vector2f mid2, vector2f h2
);

int obb_aabb_overlap(
  vector2f c, vector2f hx, vector2f hy,
  vector2f mid, vector2f h
);

#endif