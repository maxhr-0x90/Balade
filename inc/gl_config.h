/* Module de configuration d'OpenGL */

#ifndef GL_CONFIG_H_
#define GL_CONFIG_H_

/* Variable globale */
extern float side_g;
extern int div_forest_g;
extern int forest_sparsity_g;
extern int lod_g;
extern int ktree_density_g;
extern int ktree_depth_g;
extern float sight_g;

// Initialise la configuration
void init_config(int seed);

// Lance la fenêtre
void launch_window(int argc, char *argv[]);

#endif