#ifndef TEST_H_
#define TEST_H_

#include "transform_3d.h"

void test_triangle_AABB_intersection();

void init_test_octree();
void test_octree();
void test_octree_frutum(trans_3d proj);
void test_octree_final(trans_3d proj);

void test_set();

#endif