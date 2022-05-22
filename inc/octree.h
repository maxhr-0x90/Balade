/* Module gérant les octabres */

#ifndef OCTREE_H_
#define OCTREE_H_

#include "k-arbre.h"
#include "gen_array.h"
#include "vector.h"
#include "frustum.h"
#include "player.h"

// Structure des octabres
typedef struct octree_s *octree;
struct octree_s {
  vector3f c000, c111;
  karbre tree;
};

// Initalise un octabre de densité d'objet
octree object_density(
  array instances, int max_density, int max_depth, vector3f c000, vector3f c111
);

// Dessine un octrabre
void octree_render(octree ot);

// Dessine un octabre en fonction d'un frustum
void octree_render_frustum(octree ot, frustum f);

// Renvoie les masques d'instance de modèle dont le rendu est nécéssaire
// en fonction du frustum
array octree_renderable_meshes(octree ot, frustum f);

#endif