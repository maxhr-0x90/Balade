#ifndef FRUSTUM_H_
#define FRUSTUM_H_

/* Module gérant les frustums */

#include "transform_3d.h"
#include "plane.h"

#define INSIDE 2
#define INTERSECT 1
#define OUTSIDE 0

// Structure du frustum
typedef struct frustum_s *frustum;
struct frustum_s{
  plane hs[6];
};

// Initialise un frustum
frustum frustum_init(trans_3d proj, trans_3d view);

// Libère l'espace occupé par un frustum
void frustum_free(frustum f);

// Renvoie 2 si le frustum est à l'intérireur d'une AABB
//         1 si il y a une intersection entre une AABB et le frustum
//         0 sinon
int frustum_AABB_intersect(frustum f, vector3f c000, vector3f c111);

#endif