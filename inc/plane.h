/* Module gérant les plans */

#ifndef PLANE_H_
#define PLANE_H_

#include "vector.h"

/* Plan d'équation ax + by + cz + d = 0 */
typedef struct plane_s *plane;
struct plane_s{
  vector3f n;
  float d;
};

// Initialise un plan
plane plane_init(vector3f norm, float d);

// Libère l'espace occupé par un plan
void plane_free(plane p);

#endif