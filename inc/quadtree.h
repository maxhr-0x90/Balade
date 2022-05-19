#ifndef QUADTREE_H_
#define QUADTREE_H_

#include "k-arbre.h"
#include "vector.h"
#include "gen_array.h"
#include "player.h"

typedef struct quadtree_s *quadtree;
struct quadtree_s {
  vector2f c00, c11;
  karbre tree;
};

quadtree collision_tree(
  array hitboxes, int max_density, int max_depth, 
  vector2f c00, vector2f c11
);

int quadtree_check_collisions(quadtree qt, player p);
void quadtree_render(quadtree qt, float zmin, float zmax);

#endif