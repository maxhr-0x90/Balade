#ifndef COLLISION_H_
#define COLLISION_H_

#include "vector.h"

int triangle_AABB_overlap(
  vector3f t0, vector3f t1, vector3f t2,
  vector3f c000, vector3f c111
);


#endif