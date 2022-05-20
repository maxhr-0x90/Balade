#ifndef MAP_GENERATOR_H_
#define MAP_GENERATOR_H_

#include "gen_array.h"

array generate_tree_positions(int divx, int divy, int R, float density, float width, float height);
void generate_forest(float side, int div, int tree_sparsity, float tree_density, int lod);
array get_forest_meshes();
array get_forest_ground_hitboxes();

#endif