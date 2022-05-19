#ifndef OCTREE_H_
#define OCTREE_H_

#include "k-arbre.h"
#include "gen_array.h"
#include "vector.h"
#include "frustum.h"
#include "player.h"

typedef struct octree_s *octree;
struct octree_s {
  vector3f c000, c111;
  karbre tree;
};

octree object_density(
  array instances, int max_density, int max_depth, vector3f c000, vector3f c111
);
void octree_render(octree ot);
void octree_render_frustum(octree ot, frustum f);
array octree_renderable_meshes(octree ot, frustum f);

#endif