#include "../inc/quadtree.h"
#include "../inc/hitbox.h"
#include "../inc/collision.h"
#include "../inc/alloc.h"
#include "../inc/gl_util.h"

#include <stdio.h>

karbre collision_tree_rec(
  array hitboxes, int max_density, int depth, int max_depth, 
  vector2f c00, vector2f c11
);

quadtree collision_tree(
  array hitboxes, int max_density, int max_depth, 
  vector2f c00, vector2f c11
){
  if (max_density <= 0 || max_depth < 0 || hitboxes == NULL){ return NULL; }

  quadtree qt = safe_alloc(sizeof(struct quadtree_s));

  array hbs = array_init(array_size(hitboxes));
  for (int i = 0; i < array_size(hitboxes); i++){
    array_add(array_get(i, hitboxes), hbs);
  }
  
  vec2_cpy(c00, qt->c00);
  vec2_cpy(c11, qt->c11);
  qt->tree = collision_tree_rec(hbs, max_density, 0, max_depth, c00, c11);

  array_free(hbs);

  return qt;
}

karbre collision_tree_rec(
  array hitboxes, int max_density, int depth, int max_depth, 
  vector2f c00, vector2f c11
) {
  array aabb_hitboxes = array_init(array_size(hitboxes));

  int density = 0;
  int found_outer = 0;
  for (int i = 0; i < array_size(hitboxes); i++){
    hitbox hb = array_get(i, hitboxes);
    int overlap = 0;

    vector2f aabb_center = {(c00[0] + c11[0]) / 2, (c00[1] + c11[1]) / 2};
    vector2f aabb_half = {c11[0] - aabb_center[0], c11[1] - aabb_center[1]};

    switch (hb->shell){
    case CIRCLE:{
      vector2f center = {hb->values[0], hb->values[1]};
      float radius = hb->values[2];
      transformv2(hb->mat, center);
      overlap = circle_aabb_overlap(center, radius, aabb_center, aabb_half);
      break;
    }

    case AABB:{
      vector2f hb_center = {hb->values[0], hb->values[1]};
      vector2f hb_halfx = {hb->values[2], 0};
      vector2f hb_halfy = {0, hb->values[3]};
      transformv2(hb->mat, hb_center);
      transformv2(hb->mat, hb_halfx);
      transformv2(hb->mat, hb_halfy);
      overlap = obb_aabb_overlap(hb_center, hb_halfx, hb_halfy, aabb_center, aabb_half);
      break;
    }
    }

    if (overlap){
      array_add(hb, aabb_hitboxes);
      if (!found_outer && hb->domain == OUTER){ 
        found_outer = 1; 
      } else {
        density++;
      }
    }
  }

  if (density <= max_density || depth == max_depth){
    return kConsArbre(0, aabb_hitboxes);
  }

  float halfx = (c00[0] + c11[0]) / 2;
  float halfy = (c00[1] + c11[1]) / 2;

  vector2f c20 = {halfx, c00[1]};
  vector2f c02 = {c00[0], halfy};

  vector2f c22 = {halfx, halfy};

  vector2f c12 = {c11[0], halfy};
  vector2f c21 = {halfx, c11[1]};

  karbre fils = kConsArbre(
    4, NULL, 
    collision_tree_rec(aabb_hitboxes, max_density, depth + 1, max_depth, c02, c21),
    collision_tree_rec(aabb_hitboxes, max_density, depth + 1, max_depth, c22, c11),
    collision_tree_rec(aabb_hitboxes, max_density, depth + 1, max_depth, c20, c12),
    collision_tree_rec(aabb_hitboxes, max_density, depth + 1, max_depth, c00, c22)
  );

  array_free(aabb_hitboxes);
  
  return fils;
}

void karbre_flat_render(karbre a, float zmin, float zmax, vector2f c00, vector2f c11);

void quadtree_render(quadtree qt, float zmin, float zmax){
  if (qt == NULL){ return; }

  karbre_flat_render(qt->tree, zmin, zmax, qt->c00, qt->c11);
}

void karbre_flat_render(karbre a, float zmin, float zmax, vector2f c00, vector2f c11){
  if (a->k == 0){
    glColor3f(0, 1, 1);
    vector3f c000 = {c00[0], c00[1], zmin};
    vector3f c111 = {c11[0], c11[1], zmax};
    draw_aabb(c000, c111);
    return;
  }

  float halfx = (c00[0] + c11[0]) / 2;
  float halfy = (c00[1] + c11[1]) / 2;

  vector2f c20 = {halfx, c00[1]};
  vector2f c02 = {c00[0], halfy};

  vector2f c22 = {halfx, halfy};

  vector2f c12 = {c11[0], halfy};
  vector2f c21 = {halfx, c11[1]};

  karbre_flat_render(kFils(0, a), zmin, zmax, c02, c21);
  karbre_flat_render(kFils(1, a), zmin, zmax, c22, c11);
  karbre_flat_render(kFils(2, a), zmin, zmax, c20, c12);
  karbre_flat_render(kFils(3, a), zmin, zmax, c00, c22);
}

int karbre_check_collisions(karbre a, vector2f pos, float radius, vector2f c00, vector2f c11);

int quadtree_check_collisions(quadtree qt, player p){
  if (qt == NULL || p == NULL){ return 0; }

  vector2f pos = {p->pos[0], p->pos[1]};
  addv2(pos, p->speed, pos);

  if (
    pos[0] < qt->c00[0] || pos[0] > qt->c11[0] || 
    pos[1] < qt->c00[1] || pos[1] > qt->c11[1]
  ){
    return 0;
  }

  return karbre_check_collisions(qt->tree, pos, p->radius, qt->c00, qt->c11);
}

int karbre_check_collisions(karbre a, vector2f pos, float radius, vector2f c00, vector2f c11){
  if (a->k == 0){
    array hitboxes = kRacine(a);

    if (array_size(hitboxes) == 0){ return 0; }
    int all_inner = 1;
    int any_outer = 0;

    for (int i = 0; i < array_size(hitboxes); i++){
      hitbox hb = array_get(i, hitboxes);
      trans_2d inv;
      inverse2d(hb->mat, inv);
      vector2f inv_pos;
      vec2_cpy(pos, inv_pos);
      transformv2(inv, inv_pos);

      if (hb->domain == INNER){
        if (check_for_collision(inv_pos, radius, hb)){
          all_inner = 0;
        }
      } else {
        if (!check_for_collision(inv_pos, radius, hb)){
          any_outer = 1;
        }
      }
    }

    return (all_inner && any_outer);
  }

  float halfx = (c00[0] + c11[0]) / 2;
  float halfy = (c00[1] + c11[1]) / 2;

  vector2f vmin, vmax;

  int chld_num = 0;
  if (pos[0] <= halfx){
    vmin[0] = c00[0];
    vmax[0] = halfx;
  } else {
    chld_num = 1;
    vmin[0] = halfx;
    vmax[0] = c11[0];
  }

  if (pos[1] <= halfy){
    chld_num ^= 3;
    vmin[1] = c00[1];
    vmax[1] = halfy;
  } else {
    vmin[1] = halfy;
    vmax[1] = c11[1];
  }

  return karbre_check_collisions(kFils(chld_num, a), pos, radius, vmin, vmax);
}