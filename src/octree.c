#include "../inc/octree.h"
#include "../inc/collision.h"
#include "../inc/model.h"
#include "../inc/gl_util.h"
#include "../inc/alloc.h"

#include <stdlib.h>
#include <stdio.h>

karbre object_density_rec(
  array partials, int max_density, int depth, int max_depth, 
  vector3f c000, vector3f c111
);


octree object_density(
  array instances, int max_density, int max_depth, 
  vector3f c000, vector3f c111
){
  if (max_density <= 0 || max_depth < 0 || instances == NULL){ return NULL; }

  octree ot = safe_alloc(sizeof(struct octree_s));

  array partials = array_init(array_size(instances));
  for (int i = 0; i < array_size(instances); i++){
    array_add(partial_init(array_get(i, instances)), partials);
  }
  
  vec3_cpy(c000, ot->c000);
  vec3_cpy(c111, ot->c111);
  ot->tree = object_density_rec(partials, max_density, 0, max_depth, c000, c111);

  for (int i = 0; i < array_size(partials); i++){
    partial_free(array_get(i, partials));
  }
  
  array_free(0, partials);

  return ot;
}

karbre object_density_rec(
  array partials, int max_density, int depth, int max_depth, 
  vector3f c000, vector3f c111
){
  array aabb_partials = array_init(array_size(partials));
  partial mask;
  int density = 0;
  for (int i = 0; i < array_size(partials); i++){
    mask = partial_init(((partial)array_get(i, partials))->parent);
    
    for (int part = 0; part < array_size(mask->parent->mod->parts); part++){
      mesh me = array_get(part, mask->parent->mod->parts);
      int overlap_found = 0;

      for (int tri = 0; tri < array_size(me->faces); tri++){
        face f = array_get(tri, me->faces);

        if (triangle_AABB_overlap(
          array_get(f->vertices[0], me->vertices),
          array_get(f->vertices[1], me->vertices),
          array_get(f->vertices[2], me->vertices),
          c000, c111
        )){
          overlap_found = 1;
          break;
        }
      }

      if (overlap_found){ set_add(part, mask->mask); density++; overlap_found = 0; }
    }

    if (set_is_empty(mask->mask)){
      partial_free(mask);
    } else {
      array_add(mask, aabb_partials);
    }
  }


  if (density <= max_density || depth == max_depth){
    return kConsArbre(0, aabb_partials);
  }

  float halfx = (c000[0] + c111[0]) / 2;
  float halfy = (c000[1] + c111[1]) / 2;
  float halfz = (c000[2] + c111[2]) / 2;

  vector3f c200 = {halfx, c000[1], c000[2]};
  vector3f c020 = {c000[0], halfy, c000[2]};
  vector3f c002 = {c000[0], c000[1], halfz};
  vector3f c022 = {c000[0], halfy, halfz};
  vector3f c202 = {halfx, c000[1], halfz};
  vector3f c220 = {halfx, halfy, c000[2]};

  vector3f c222 = {halfx, halfy, halfz};

  vector3f c122 = {c111[0], halfy, halfz};
  vector3f c212 = {halfx, c111[1], halfz};
  vector3f c221 = {halfx, halfy, c111[2]};
  vector3f c211 = {halfx, c111[1], c111[2]};
  vector3f c121 = {c111[0], halfy, c111[2]};
  vector3f c112 = {c111[0], c111[1], halfz};

  karbre fils = kConsArbre(
    8, NULL, 
    object_density_rec(aabb_partials, max_density, depth + 1, max_depth, c002, c221),
    object_density_rec(aabb_partials, max_density, depth + 1, max_depth, c202, c121),
    object_density_rec(aabb_partials, max_density, depth + 1, max_depth, c200, c122),
    object_density_rec(aabb_partials, max_density, depth + 1, max_depth, c000, c222),
    object_density_rec(aabb_partials, max_density, depth + 1, max_depth, c022, c211),
    object_density_rec(aabb_partials, max_density, depth + 1, max_depth, c222, c111),
    object_density_rec(aabb_partials, max_density, depth + 1, max_depth, c220, c112),
    object_density_rec(aabb_partials, max_density, depth + 1, max_depth, c020, c212)
  );

  for (int i = 0; i < array_size(aabb_partials); i++){
    partial_free(array_get(i, aabb_partials));
  }
  
  array_free(0, aabb_partials);
  
  return fils;
}


void karbre_render(karbre a, vector3f c000, vector3f c111);

void octree_render(octree ot){
  if (ot == NULL){ return; }

  karbre_render(ot->tree, ot->c000, ot->c111);

  glColor3f(1, 1, 1);
  draw_cube(ot->c000, ot->c111);
}

void karbre_render(karbre a, vector3f c000, vector3f c111){
  if (a->k == 0){
    glColor3f(1, 0, 0);
    draw_cube(c000, c111);
    return;
  }

  float halfx = (c000[0] + c111[0]) / 2;
  float halfy = (c000[1] + c111[1]) / 2;
  float halfz = (c000[2] + c111[2]) / 2;

  vector3f c200 = {halfx, c000[1], c000[2]};
  vector3f c020 = {c000[0], halfy, c000[2]};
  vector3f c002 = {c000[0], c000[1], halfz};
  vector3f c022 = {c000[0], halfy, halfz};
  vector3f c202 = {halfx, c000[1], halfz};
  vector3f c220 = {halfx, halfy, c000[2]};

  vector3f c222 = {halfx, halfy, halfz};

  vector3f c122 = {c111[0], halfy, halfz};
  vector3f c212 = {halfx, c111[1], halfz};
  vector3f c221 = {halfx, halfy, c111[2]};
  vector3f c211 = {halfx, c111[1], c111[2]};
  vector3f c121 = {c111[0], halfy, c111[2]};
  vector3f c112 = {c111[0], c111[1], halfz};

  karbre_render(kFils(0, a), c002, c221);
  karbre_render(kFils(1, a), c202, c121);
  karbre_render(kFils(2, a), c200, c122);
  karbre_render(kFils(3, a), c000, c222);
  karbre_render(kFils(4, a), c022, c211);
  karbre_render(kFils(5, a), c222, c111);
  karbre_render(kFils(6, a), c220, c112);
  karbre_render(kFils(7, a), c020, c212);
}
