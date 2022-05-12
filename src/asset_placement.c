#include "GL/gl.h"
#include "GL/glut.h"

#include "../inc/asset_placement.h"

void place_model(mod_inst mod){
  if (mod.mod == NULL) { return; }

  glPushMatrix();
  glLoadMatrixf(mod.mat);
  for (int i = 0; i < array_size(mod.mod->parts); i++){
    mesh me = (mesh)array_get(i, mod.mod->parts);

    glBegin(GL_TRIANGLES);

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, me->material.diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, me->material.specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, me->material.shininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, me->material.ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, me->material.emission);

    for (int j = 0; j < array_size(me->faces); j++){
      face f = (face)array_get(j, me->faces);
      glNormal3fv(((GLfloat *)array_get(f->normal, me->normals)));
      glVertex3fv(((GLfloat *)array_get(f->vertices[0], me->vertices)));
      glVertex3fv(((GLfloat *)array_get(f->vertices[1], me->vertices)));
      glVertex3fv(((GLfloat *)array_get(f->vertices[2], me->vertices)));
    }

    glEnd();
  }
  glPopMatrix();
}

void place_light(light l, GLenum num){
  glLightfv(num, GL_AMBIENT, l.ambient);
  glLightfv(num, GL_DIFFUSE, l.diffuse);
  glLightfv(num, GL_SPECULAR, l.specular);
  glLightfv(num, GL_POSITION, l.position);
  glLightfv(num, GL_SPOT_DIRECTION, l.direction);

  glLightf(num, GL_SPOT_CUTOFF, l.spot_cutoff);
  glLightf(num, GL_SPOT_EXPONENT, l.spot_exponent);
  
  glLightf(num, GL_CONSTANT_ATTENUATION, 0);
  glLightf(num, GL_LINEAR_ATTENUATION, 0);
  glLightf(num, GL_QUADRATIC_ATTENUATION, 0);
  switch (l.attenuation){
  case CONSTANT:
    glLightf(num, GL_CONSTANT_ATTENUATION, 1);
    break;

  case LINEAR:
    glLightf(num, GL_LINEAR_ATTENUATION, 1);
    break;

  case QUADRATIQUE:
    glLightf(num, GL_QUADRATIC_ATTENUATION, 1);
    break;
  }
}