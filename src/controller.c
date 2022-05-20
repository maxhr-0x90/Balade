#include "../inc/controller.h"

#include "GL/gl.h"
#include "GL/glut.h"

#include "../inc/octree.h"
#include "../inc/quadtree.h"
#include "../inc/frustum.h"
#include "../inc/map_generator.h"
#include "../inc/asset_placement.h"

#include <stdlib.h>
#include <stdio.h>

void init_noise(int seed);
void init_ktrees(float side, int density, int depth);

struct osn_context *ctx_g = NULL;

octree density_g;
quadtree ground_collision_g, itp_collision_g;
frustum frustum_g;
level lvl_g = GROUND;

void init_assets(
  int seed, float forest_side, int forest_div, int forest_sparsity, float forest_density, int lod, 
  int ktree_density, int ktree_depth
){
  init_noise(seed);

  generate_forest(forest_side, forest_div, forest_sparsity, forest_density, lod);

  init_ktrees(forest_side, ktree_density, ktree_depth);
}

void init_noise(int seed){
  if (ctx_g != NULL){ open_simplex_noise_free(ctx_g); }
  open_simplex_noise(seed, &ctx_g);
}

struct osn_context *get_noise_context(){
  return ctx_g;
}

void init_ktrees(float side, int density, int depth){
  // Octree
  vector3f c000 = {side * -.55f, side * -.55f, side * -.55f};
  vector3f c111 = {side * .55f, side * .55f, side * .55f};
  density_g = object_density(get_forest_meshes(), density, depth, c000, c111);

  // Quadtrees
  vector2f c00 = {-side * .55f, -side * .55f};
  vector2f c11 = {side * .55f, side * .55f};
  ground_collision_g = collision_tree(get_forest_ground_hitboxes(), density, depth, c00, c11);
  itp_collision_g = collision_tree(get_forst_itp_hitboxes(), density * 4, depth, c00, c11);
}

void render_world(int wired, ktree_type ktree, trans_3d proj, player p){
  frustum_g = frustum_init(proj, NULL);
  array rends = octree_renderable_meshes(density_g, frustum_g);

  if (wired){
    for (int i = 0; i < array_size(rends); i++){
      glColor3f(1, 1, 1);
      place_wire_partial(array_get(i, rends));
    }
  } else {
    for (int i = 0; i < array_size(rends); i++){
      place_partial(array_get(i, rends));
    }
  }

  array hitboxes = NULL;
  quadtree hb_quad = NULL;
  switch (ktree){
  case QUADTREE:
    switch (lvl_g){
    case GROUND:
      hitboxes = get_forest_ground_hitboxes();
      hb_quad = ground_collision_g;
      break;

    case CLIMB: break;
    case ITP: 
      hitboxes = get_forst_itp_hitboxes();
      hb_quad = itp_collision_g;
      break;
    }

    if (hitboxes == NULL || hb_quad == NULL){ break; }
    quadtree_render(ground_collision_g, p->pos[2] - 2, p->pos[2] - 1);
    for (int i = 0; i < array_size(hitboxes); i++){
      glColor3f(0, 1, 0);
      render_hitbox(array_get(i, hitboxes), p->pos[2] - 2, p->pos[2] - 1);
      glColor3f(1, 1, 1);
    }
    break;

  case OCTREEE: 
    octree_render_frustum(density_g, frustum_g);
    break;

  case NONE: break;
  }

  array_free(rends);
  frustum_free(frustum_g);
}

hb_type update_player_position(player p){
  quadtree hb_quad = NULL;

  switch (lvl_g){
    case GROUND:
      hb_quad = ground_collision_g;
      break;

    case CLIMB: return TREE;
    case ITP: 
      hb_quad = itp_collision_g;
      break;
  }

  hb_type type;
  if (quadtree_check_collisions(hb_quad, p)){
    step(p);
    return NO_HB;
  } else {
    type = quadtree_collision_type(hb_quad, p);
  }

  vector2f speed;
  vec2_cpy(p->speed, speed);
  p->speed[1] = 0;

  if (quadtree_check_collisions(hb_quad, p)){
    step(p);
    return type;
  } 

  vec2_cpy(speed, p->speed);
  p->speed[0] = 0;

  if (quadtree_check_collisions(hb_quad, p)){
    step(p);
  } 

  return type;
}

void set_elevation(level lvl){
  lvl_g = lvl;
}