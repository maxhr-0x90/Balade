#include "../inc/octree.h"
#include "../inc/collision.h"
#include "../inc/model.h"
#include "../inc/gl_util.h"
#include "../inc/alloc.h"
#include "../inc/hitbox.h"

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
  mod_inst parent;
  int density = 0;
  for (int i = 0; i < array_size(partials); i++){
    parent = ((partial)array_get(i, partials))->parent;
    mask = partial_init(parent);
    
    for (int part = 0; part < array_size(mask->parent->mod->parts); part++){
      mesh me = array_get(part, mask->parent->mod->parts);
      int overlap_found = 0;

      for (int tri = 0; tri < array_size(me->faces); tri++){
        face f = array_get(tri, me->faces);
        vector3f p0, p1, p2;

        vec3_cpy(array_get(f->vertices[0], me->vertices), p0);
        vec3_cpy(array_get(f->vertices[1], me->vertices), p1);
        vec3_cpy(array_get(f->vertices[2], me->vertices), p2);

        transformv3(parent->mat, p0);
        transformv3(parent->mat, p1);
        transformv3(parent->mat, p2);

        if (triangle_AABB_overlap(p0, p1, p2, c000, c111)){
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
}

void karbre_render(karbre a, vector3f c000, vector3f c111){
  if (a->k == 0){
    glColor3f(1, 0, 0);
    draw_aabb(c000, c111);
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


void karbre_render_frustum(karbre a, frustum f, vector3f c000, vector3f c111);

void octree_render_frustum(octree ot, frustum f){
  if (ot == NULL || f == NULL){ return; }

  karbre_render_frustum(ot->tree, f, ot->c000, ot->c111);
  glColor3f(1, 1, 0);
  draw_aabb(ot->c000, ot->c111);
}

void karbre_render_frustum(karbre a, frustum f, vector3f c000, vector3f c111){
  if (frustum_AABB_intersect(f, c000, c111) == OUTSIDE){ return; }
  if (a->k == 0){
    glColor3f(1, 0, 0);
    draw_aabb(c000, c111);
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

  karbre_render_frustum(kFils(0, a), f, c002, c221);
  karbre_render_frustum(kFils(1, a), f, c202, c121);
  karbre_render_frustum(kFils(2, a), f, c200, c122);
  karbre_render_frustum(kFils(3, a), f, c000, c222);
  karbre_render_frustum(kFils(4, a), f, c022, c211);
  karbre_render_frustum(kFils(5, a), f, c222, c111);
  karbre_render_frustum(kFils(6, a), f, c220, c112);
  karbre_render_frustum(kFils(7, a), f, c020, c212);
}

array karbre_renderable_meshes(karbre a, frustum f, vector3f c000, vector3f c111);
void fuse_child(array child, array dest);
void fuse_partials(array add, array dest);

array octree_renderable_meshes(octree ot, frustum f){
  if (ot == NULL || f == NULL){ return NULL; }

  return karbre_renderable_meshes(ot->tree, f, ot->c000, ot->c111);
}

array karbre_renderable_meshes(karbre a, frustum f, vector3f c000, vector3f c111){
  if (frustum_AABB_intersect(f, c000, c111) == OUTSIDE){ return NULL; }
  if (a->k == 0){
    array partials = array_init(4);
    fuse_partials(kRacine(a), partials);
    return partials;
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

  array child;
  array partials = array_init(32);

  child = karbre_renderable_meshes(kFils(0, a), f, c002, c221);
  fuse_child(child, partials);

  child = karbre_renderable_meshes(kFils(1, a), f, c202, c121);
  fuse_child(child, partials);

  child = karbre_renderable_meshes(kFils(2, a), f, c200, c122);
  fuse_child(child, partials);

  child = karbre_renderable_meshes(kFils(3, a), f, c000, c222);
  fuse_child(child, partials);

  child = karbre_renderable_meshes(kFils(4, a), f, c022, c211);
  fuse_child(child, partials);

  child = karbre_renderable_meshes(kFils(5, a), f, c222, c111);
  fuse_child(child, partials);

  child = karbre_renderable_meshes(kFils(6, a), f, c220, c112);
  fuse_child(child, partials);

  child = karbre_renderable_meshes(kFils(7, a), f, c020, c212);
  fuse_child(child, partials);

  return partials;
}

void fuse_child(array child, array dest){
  if (child == NULL){ return; }

  fuse_partials(child, dest);
  for (int i = 0; i < array_size(child); i++){ partial_free(array_get(i, child)); }
  array_free(0, child);
}

void fuse_partials(array add, array dest){
  for (int i = 0; i < array_size(add); i++) {
    int already_in = 0;
    for (int j = 0; j < array_size(dest); j++) {
      if (((partial)array_get(j, dest))->parent == ((partial)array_get(i, add))->parent){
        already_in = 1;
        partial_fuse(array_get(i, add), array_get(j, dest));
      }
    }

    if (!already_in){
      partial cpy = partial_init(((partial)array_get(i, add))->parent);
      partial_fuse(array_get(i, add), cpy);
      array_add(cpy, dest);
    }
  }
}