/* Module de génération de la carte 3D */

#ifndef MAP_GENERATOR_H_
#define MAP_GENERATOR_H_

#include "gen_array.h"

// Génère les position des arbres
array generate_tree_positions(int divx, int divy, int R, float density, float width, float height);

// Génère la forêt
void generate_forest(float side, int div, int tree_sparsity, float tree_density, int lod);


// Récupère les meshs de la forết
array get_forest_meshes();

// Récupère les hitboxs de la forêt au sol
array get_forest_ground_hitboxes();

// Récupère les hitboxs de la forêt au dans les arbre (in the pines : itp)
array get_forst_itp_hitboxes();

#endif