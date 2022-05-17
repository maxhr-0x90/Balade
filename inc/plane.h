#ifndef PLANE_H_
#define PLANE_H_

#include "vector.h"

/* Plan d'équation ax + by + cz + d = 0 */
typedef struct plane_s *plane;
struct plane_s{
  vector3f n;
  float d;
};

plane plane_init(vector3f norm, float d);
void plane_free(plane p);

#endif