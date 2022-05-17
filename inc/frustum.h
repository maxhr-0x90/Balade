#ifndef FRUSTUM_H_
#define FRUSTUM_H_

#include "transform_3d.h"
#include "plane.h"

#define INSIDE 2
#define INTERSECT 1
#define OUTSIDE 0

typedef struct frustum_s *frustum;
struct frustum_s{
  plane hs[6];
};

frustum frustum_init(trans_3d proj, trans_3d view);
void frustum_free(frustum f);
int frustum_AABB_intersect(frustum f, vector3f c000, vector3f c111);

#endif