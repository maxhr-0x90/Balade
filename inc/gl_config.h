#ifndef GL_CONFIG_H_
#define GL_CONFIG_H_

extern float side_g;
extern int div_forest_g;
extern int forest_sparsity_g;
extern int lod_g;
extern int ktree_density_g;
extern int ktree_depth_g;
extern float sight_g;

void init_config(int seed);
void launch_window(int argc, char *argv[]);

#endif