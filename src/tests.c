#include "../inc/tests.h"

#include "GL/gl.h"
#include "GL/glut.h"

#include <unistd.h>
#include <stdio.h>

#include "../inc/gl_util.h"
#include "../inc/collision.h"

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

    draw_cube(aabbs[i][0], aabbs[i][1]);
  }
  
}