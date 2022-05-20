#ifndef TEST_H_
#define TEST_H_

#include "transform_3d.h"
#include "player.h"

void test_triangle_AABB_intersection();

void init_test_octree();
void test_octree();
void test_octree_frutum(trans_3d proj);
void test_octree_final(trans_3d proj);

void init_test_quadtree();
void test_collision(player p);
void test_collision_render(player p);

void render_all_instances();

void test_set();
void test_transformation();
void test_graph();
void test_gen_pos();

#endif