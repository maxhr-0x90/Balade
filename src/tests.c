#include "../inc/tests.h"

#include "GL/gl.h"
#include "GL/glut.h"

#include <unistd.h>
#include <stdio.h>
#include <math.h>

#include "../inc/gl_util.h"
#include "../inc/collision.h"
#include "../inc/model.h"
#include "../inc/model_loader.h"
#include "../inc/model_generator.h"
#include "../inc/gen_array.h"
#include "../inc/octree.h"
#include "../inc/quadtree.h"
#include "../inc/asset_placement.h"
#include "../inc/set.h"
#include "../inc/transform_2d.h"

void test_triangle_AABB_intersection(){
  vector3f tri[3] = {{-1.0f, 5.0f, 0.0f}, {2.0f, 2.0f, -3.0f}, {5.0f, 5.0f, 0.0f}};
  vector3f aabbs[6][2] = {
    // Normalement intersection
    {{1.5f, 3.5f, -1.5f}, {2.5f, 4.5f, -.5f}},
    {{-1.5f, 4.5f, -.5f}, {-.5f, 5.5f, 0.5f}},
    {{-5.0f, -7.0f, -5.0f}, {5.0f, 7.0f, 5.0f}},
    {{-1.0f, 2.5f, -5.0f}, {5.0f, 3.5f, -1.0f}},
    // Noramalement sans intersection 
    {{1.0f, 1.0f, -.5f}, {4.0f, 4.0f, 2.5f}},
    {{-3.0f, -3.0f, -3.0f}, {-1.0f, -1.0f, -1.0f}}
  };

  draw_axes(1.0f);

  // Triangle
  glColor3f(0, 0, 1);
  glBegin(GL_LINE_LOOP);
  glVertex3fv(tri[0]);
  glVertex3fv(tri[1]);
  glVertex3fv(tri[2]);
  glEnd();

  // AABBs
  for (int i = 0; i < 6; i++){
    if (triangle_AABB_overlap(tri[0], tri[1], tri[2], aabbs[i][0], aabbs[i][1])){
      glColor3f(0, 1, 0);
    } else {
      glColor3f(1, 0, 0);
    }

    draw_aabb(aabbs[i][0], aabbs[i][1]);
  }
}

array instances_g, hitboxes_g;
octree tree_g;
quadtree collision_g;

void init_test_octree(){
  instances_g = array_init(8);

  model p = ground_model(100, 100, 10, 10);
  model pine = load_model("models/pine.swag");

  mod_inst p2 = inst_init(pine);
  trans_translate(-10, 15, -2, p2->mat);

  array_add(inst_init(p), instances_g);
  array_add(inst_init(pine), instances_g);
  array_add(p2, instances_g);

  vector3f c000 = {-20, -20, -10};
  vector3f c111 = {20, 20, 30};
  tree_g = object_density(instances_g, 1, 4, c000, c111);
}

void test_octree(){
  for (int i = 0; i < array_size(instances_g); i++){
    glColor3f(1, 1, 1);
    place_wire_model(array_get(i, instances_g));
  }
  
  octree_render(tree_g);
}

frustum frustum_g;

void test_octree_frutum(trans_3d proj){
  frustum_g = frustum_init(proj, NULL);
  
  octree_render_frustum(tree_g, frustum_g);

  frustum_free(frustum_g);
}

void test_octree_final(trans_3d proj){
  frustum_g = frustum_init(proj, NULL);

  array rend = octree_renderable_meshes(tree_g, frustum_g);
  for (int i = 0; i < array_size(rend); i++){
    glColor3f(1, 1, 1);
    place_wire_partial(array_get(i, rend));
  }

  octree_render_frustum(tree_g, frustum_g);

  for (int i = 0; i < array_size(rend); i++){ partial_free(array_get(i, rend)); }
  array_free(0, rend);
  frustum_free(frustum_g);
}

void init_test_quadtree(){
  hitboxes_g = array_init(8);
  float vals[] = {0, 0, 20, 20};
  hitbox hb = hitbox_init(AABB, OUTER, vals);
  array_add(hb, hitboxes_g);

  float valss[] = {15, 0, 20, 10};
  hb = hitbox_init(AABB, OUTER, valss);
  array_add(hb, hitboxes_g);

  float vals2[] = {0, 0, 2};
  hb = hitbox_init(CIRCLE, INNER, vals2);
  array_add(hb, hitboxes_g);

  hb = hitbox_init(CIRCLE, INNER, vals2);
  trans2d_translate(5, 10, hb->mat);
  array_add(hb, hitboxes_g);

  vector2f c00 = {-50, -50};
  vector2f c11 = {50, 50};
  collision_g = collision_tree(hitboxes_g, 1, 4, c00, c11);
}

void test_collision_render(player p){
  //quadtree_render(collision_g, p->pos[2] - 1, p->pos[2] + 1);
  for (int i = 0; i < array_size(hitboxes_g); i++){
    glColor3f(0, 1, 0);
    render_hitbox(array_get(i, hitboxes_g), p->pos[2] - 1, p->pos[2] + 1);
    glColor3f(1, 1, 1);
  }
}

void test_collision(player p){
  if (quadtree_check_collisions(collision_g, p)){
    step(p);
    return;
  } 

  vector2f speed;
  vec2_cpy(p->speed, speed);
  p->speed[1] = 0;

  if (quadtree_check_collisions(collision_g, p)){
    step(p);
    return;
  } 

  vec2_cpy(speed, p->speed);
  p->speed[0] = 0;

  if (quadtree_check_collisions(collision_g, p)){
    step(p);
    return;
  } 
}


void test_set(){
  set s = set_init(50);
  set s2 = set_init(50);
  set s3 = set_init(50);

  printf("is empty %s\n", set_is_empty(s) ? "true" : "false");

  set_add(3, s);
  set_add(0, s);
  set_add(10, s);
  set_add(9, s);
  set_add(44, s);
  set_add(20, s);
  set_add(14, s);
  set_add(32, s);
  set_add(16, s);

  printf("%d\n", set_size(s));

  for (int i = 0; i < 100; i++){
    set_add(2, s);
  }

  printf("%d\n", set_size(s));

  set_add(-23, s);
  set_add(64, s);
  set_add(1234565, s);
  set_add(-123456, s);
  
  printf("%d\n", set_size(s));

  set_rem(3, s);
  set_rem(16, s);
  set_rem(0, s);
  set_rem(1, s);
  set_rem(-567, s);

  printf("%d\n", set_size(s));

  printf("is empty %s\n", set_is_empty(s) ? "true" : "false");

  set_add(3, s2);
  set_add(44, s2);
  set_add(10, s2);
  set_add(11, s2);
  set_add(12, s2);
  set_add(13, s2);
  set_add(14, s2);
  set_add(15, s2);
  set_add(16, s2);

  printf("%d\n", set_size(s3));
  printf("%d\n", set_size(s));

  set_union(s, s2, s3);
  set_union(s, s2, s);

  printf("%d\n", set_size(s3));
  printf("%d\n", set_size(s));

  print_set(s);
  print_set(s2);
  print_set(s3);

  set_free(s);
  set_free(s2);
  set_free(s3);
}

void test_transformation(){
  trans_2d tr, inv, comb;
  trans2d_id(tr);

  printf("===========================\n");
  aff_trans_2d(tr);

  trans2d_translate(2, 5, tr);
  trans2d_rotate(M_PI_4, tr);

  printf("===========================\n");
  aff_trans_2d(tr);

  inverse2d(tr, inv);

  printf("===========================\n");
  aff_trans_2d(inv);

  trans2d_comb(inv, tr, comb);

  printf("===========================\n");
  aff_trans_2d(comb);
  printf("===========================\n");
}