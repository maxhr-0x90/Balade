/* Module gérant les quadarbres */

#ifndef QUADTREE_H_
#define QUADTREE_H_

#include "k-arbre.h"
#include "vector.h"
#include "gen_array.h"
#include "player.h"
#include "hitbox.h"

// Structure des quadarbres
typedef struct quadtree_s *quadtree;
struct quadtree_s {
  vector2f c00, c11;
  karbre tree;
};

// Initialise le quadarbre des collisions
quadtree collision_tree(
  array hitboxes, int max_density, int max_depth, 
  vector2f c00, vector2f c11
);

// Renvoie 1 s'il n'y aucune collision, 0 sinon
int quadtree_check_collisions(quadtree qt, player p);

// Affiche le quadarbre
void quadtree_render(quadtree qt, float zmin, float zmax);

// Renvoie le type de collision
hb_type quadtree_collision_type(quadtree qt, player p);

#endif