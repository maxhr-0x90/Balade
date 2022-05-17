#include "../inc/gl_util.h"

#include "GL/gl.h"
#include "GL/glut.h"

#include <unistd.h>
#include <stdio.h>

void draw_cube(vector3f c000, vector3f c111){
  glBegin(GL_LINE_LOOP);
  glVertex3f(c000[0], c000[1], c000[2]);
  glVertex3f(c000[0], c111[1], c000[2]);
  glVertex3f(c111[0], c111[1], c000[2]);
  glVertex3f(c111[0], c000[1], c000[2]);
  glEnd();

  glBegin(GL_LINE_LOOP);
  glVertex3f(c000[0], c000[1], c111[2]);
  glVertex3f(c000[0], c111[1], c111[2]);
  glVertex3f(c111[0], c111[1], c111[2]);
  glVertex3f(c111[0], c000[1], c111[2]);
  glEnd();

  glBegin(GL_LINES);

  glVertex3f(c000[0], c000[1], c000[2]);
  glVertex3f(c000[0], c000[1], c111[2]);

  glVertex3f(c000[0], c111[1], c000[2]);
  glVertex3f(c000[0], c111[1], c111[2]);

  glVertex3f(c111[0], c111[1], c000[2]);
  glVertex3f(c111[0], c111[1], c111[2]);

  glVertex3f(c111[0], c000[1], c000[2]);
  glVertex3f(c111[0], c000[1], c111[2]);

  glEnd();
}

void draw_axes(float length){
  glBegin(GL_LINES);

  glColor3f(1, 0, 0);
  glVertex3f(0, 0, 0);
  glVertex3f(length, 0, 0);

  glColor3f(0, 1, 0);
  glVertex3f(0, 0, 0);
  glVertex3f(0, length, 0);

  glColor3f(0, 0, 1);
  glVertex3f(0, 0, 0);
  glVertex3f(0, 0, length);

  glColor3f(1, 1, 1);

  glEnd();
}

void draw_frustum(trans_3d proj){
  trans_3d inv_proj;
  vector3f c000 = {-1, -1, -1};
  vector3f c111 = {1, 1, 1};

  inverse(proj, inv_proj);

  glPushMatrix();
  glLoadMatrixf(inv_proj);
  draw_cube(c000, c111);
  glPopMatrix();
}