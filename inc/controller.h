/* Module de gestion de l'execution */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "player.h"
#include "transform_3d.h"
#include "open-simplex-noise.h"
#include "hitbox.h"

typedef enum {NONE, QUADTREE, OCTREEE} ktree_type;
typedef enum {GROUND, CLIMB, ITP} level;

// Retourne le contexte du bruit
struct osn_context *get_noise_context();

// Initialise les élements 
void init_assets(
  int seed, float forest_side, int forest_div, int forest_sparsity, float forest_density, int lod, 
  int ktree_density, int ktree_depth
);

// Rendu du monde 3D
void render_world(int wired, ktree_type ktree, trans_3d proj, player p);

// Met à jour la position du joueur
hb_type update_player_position(player p);

// Défini l'élévation du joueur
void set_elevation(level lvl);

#endif