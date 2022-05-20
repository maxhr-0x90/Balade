#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "player.h"
#include "transform_3d.h"
#include "open-simplex-noise.h"

typedef enum {NONE, QUADTREE, OCTREEE} ktree_type;
typedef enum {GROUND, CLIMB, ITP} level;

struct osn_context *get_noise_context();

void init_assets(
  int seed, float forest_side, int forest_div, int forest_sparsity, float forest_density, int lod, 
  int ktree_density, int ktree_depth
);
void render_world(int wired, ktree_type ktree, trans_3d proj, player p);

#endif