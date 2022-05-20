#include "../inc/gl_config.h"

#include "GL/gl.h"
#include "GL/glut.h"

#include <unistd.h>
#include <stdio.h>
#include <math.h>

#include "../inc/voxel.h"
#include "../inc/model.h"
#include "../inc/player.h"
#include "../inc/light.h"
#include "../inc/gl_util.h"
#include "../inc/model_generator.h"
#include "../inc/asset_placement.h"
#include "../inc/model_loader.h"
#include "../inc/controller.h"

#define LARGEUR_FEN 800
#define HAUTEUR_FEN 800

#define DOWN 1
#define RIGHT 2
#define LEFT 4
#define UP 8
#define L1 16
#define R1 32
#define SHR 64
#define OPT 128
#define HOME 256

void check_state_changer(unsigned int buttonMask);
void joystick_god(unsigned int buttonMask, int x, int y, int z);
void joystick_player(unsigned int buttonMask, int x, int y, int z);

int mouse_x_g, mouse_y_g;

int fps_g = 0;
int wired_g = 0;
ktree_type ktree_g = NONE;

light light_g;
player player_g;
float radius_g, phi_g, theta_g;

int forward_g, back_g, left_g, right_g, upward_g, downward_g;
int shr_pressed_g, opt_pressed_g, home_pressed_g;

int wind_width_g = LARGEUR_FEN;
int wind_height_g = HAUTEUR_FEN;

GLdouble fovz_g = 67;
GLdouble ratio_g = 1;

void lookat_fps();
void lookat_god();
void player_update_fps();
void player_update_god();

void affichage(){
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  trans_3d proj;

  gluPerspective(fovz_g, ratio_g, 1, 50);
  lookat_fps();
  glGetFloatv(GL_PROJECTION_MATRIX, proj);

  if (!fps_g){
    glLoadIdentity();
    gluPerspective(fovz_g, ratio_g, 1, 400);
    lookat_god();
  }

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  if (!wired_g){
    place_light(light_g, GL_LIGHT0);
  }

  render_world(wired_g, ktree_g, proj, player_g);

  if (!fps_g){
    glColor3f(0, 0, 1);
    draw_frustum(proj);
    glColor3f(1, 1, 1);
  }

  glFlush();
}

void lookat_fps(){
  gluLookAt(
    player_g->pos[0], 
    player_g->pos[1], 
    player_g->pos[2], 
    player_g->pos[0] + player_g->dir[0], 
    player_g->pos[1] + player_g->dir[1], 
    player_g->pos[2] + player_g->dir[2], 
    player_g->up[0], 
    player_g->up[1], 
    player_g->up[2]
  );
}

void lookat_god(){
  float cos_phi = cosf(phi_g);
  float sin_phi = sinf(phi_g);
  float cos_theta = cosf(theta_g);
  float sin_theta = sinf(theta_g);

  gluLookAt(
    cos_phi * cos_theta * radius_g, 
    cos_phi * sin_theta * radius_g, 
    sin_phi * radius_g, 
    0, 0, 0, 
    -sin_phi * cos_theta, 
    -sin_phi * sin_theta, 
    cos_phi
  );
}

void reshape(int width, int height){
  wind_width_g = width;
  wind_height_g = height;
  ratio_g = (GLdouble)width / height;
  glViewport(0, 0, width, height);
}

void idle(){
  if (fps_g){
    player_update_fps();
  }
}

void player_update_fps(){
  if (!(forward_g | back_g | left_g | right_g | downward_g | upward_g)){ return; }

  float delta = 0.01f;

  player_g->speed[0] = 0;
  player_g->speed[1] = 0; 

  if (forward_g){
    player_g->speed[0] += cosf(player_g->theta);
    player_g->speed[1] += sinf(player_g->theta);
  } else if (back_g){
    player_g->speed[0] -= cosf(player_g->theta);
    player_g->speed[1] -= sinf(player_g->theta);
  }

  if (right_g){
    player_g->speed[0] += sinf(player_g->theta);
    player_g->speed[1] -= cosf(player_g->theta);
  } else if (left_g){
    player_g->speed[0] -= sinf(player_g->theta);
    player_g->speed[1] += cosf(player_g->theta);
  }

  if (downward_g){
    //player_g->pos[2] -= delta;
  } else if (upward_g){
    //player_g->pos[2] += delta;
  }

  if (right_g | left_g | forward_g | back_g){
    normalizev2(player_g->speed);
    multv2(player_g->speed, delta);
    step(player_g);
    setup_torch(&light_g, player_g);
  }

  glutPostRedisplay();
}

void check_state_changer(unsigned int buttonMask){
  if (!((SHR | OPT | HOME) & buttonMask)){ 
    shr_pressed_g = 0;
    opt_pressed_g = 0;
    home_pressed_g = 0;
    return; 
  }

  if (SHR & buttonMask){
    if (!shr_pressed_g){
      wired_g = !wired_g;

      if (!wired_g){
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
      } else {
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
      }

      shr_pressed_g = 1;
      glutPostRedisplay();
    }
  } else {
    shr_pressed_g = 0;
  }

  if (OPT & buttonMask){
    if (!opt_pressed_g){
      switch (ktree_g){
      case NONE: ktree_g = OCTREEE; break;
      case OCTREEE: ktree_g = QUADTREE; break;
      case QUADTREE: ktree_g = NONE; break;
      }

      opt_pressed_g = 1;
      glutPostRedisplay();
    }
  } else {
    opt_pressed_g = 0;
  }

  if (HOME & buttonMask){
    if (!home_pressed_g){
      fps_g = !fps_g;

      if (fps_g){
        glutJoystickFunc(joystick_player, 50);
      } else {
        glutJoystickFunc(joystick_god, 50);
      }

      home_pressed_g = 1;
      glutPostRedisplay();
    }
  } else {
    home_pressed_g = 0;
  }
}

void joystick_player(unsigned int buttonMask, int x, int y, int z){
  forward_g = 0;
  back_g = 0;
  left_g = 0;
  right_g = 0;
  downward_g = 0;
  upward_g = 0;

  if (buttonMask){
    if (buttonMask & DOWN){
      back_g = 1;
    } else if (buttonMask & UP){
      forward_g = 1;
    }

    if (buttonMask & LEFT){
      left_g = 1;
    } else if (buttonMask & RIGHT){
      right_g = 1;
    }

    if (buttonMask & L1){
      downward_g = 1;
    } else if (buttonMask & R1){
      upward_g = 1;
    }
  }

  if (x != 0 || y != 0){
    float angluar_speed = 0.05;
    float theta = ((-x * 0.18) * M_PI / 180) * angluar_speed;
    float phi = ((y * 0.18) * M_PI / 180) * angluar_speed;
    player_set_cam(player_g->phi + phi, player_g->theta + theta, player_g);
    setup_torch(&light_g, player_g);
    glutPostRedisplay();
  }

  check_state_changer(buttonMask);
}

void joystick_god(unsigned int buttonMask, int x, int y, int z){
  if (buttonMask & L1){
    radius_g -= 1;
    radius_g = radius_g <= 0.5 ? 0.5 : radius_g;
    glutPostRedisplay();
  } else if (buttonMask & R1){
    radius_g += 1;
    glutPostRedisplay();
  }

  if (x != 0 || y != 0){
    float angluar_speed = 0.05;
    float theta = ((x * 0.18) * M_PI / 180) * angluar_speed;
    float phi = ((-y * 0.18) * M_PI / 180) * angluar_speed;
    theta_g += theta;
    phi_g += phi;

    if (theta_g > M_PI){
      theta_g -= 2 * M_PI;
    } else if (theta < -M_PI){
      theta_g += 2 * M_PI;
    }

    if (phi_g > M_PI){
      phi_g -= 2 * M_PI;
    } else if (phi < -M_PI){
      phi_g += 2 * M_PI;
    }

    glutPostRedisplay();
  }

  check_state_changer(buttonMask);
}

float side_g = 200.0f;
int div_forest_g = 15;
int forest_sparsity_g = 1;
float forest_density_g = 1;
int lod_g = 4;
int ktree_density_g = 2;
int ktree_depth_g = 4;

void init_config(int seed){
  radius_g = 10.0f;
  phi_g = 0;
  theta_g = 0;

  player_g = player_init();
  player_set_pos(0, 0, 2, player_g);

  forward_g = 0;
  back_g = 0;
  left_g = 0;
  right_g = 0;
  downward_g = 0;
  upward_g = 0;

  shr_pressed_g = 0;
  opt_pressed_g = 0;
  home_pressed_g = 0;

  light_default(&light_g);
  light_ambient(.75, .75, .75, 1, &light_g);
  light_specular(0.7, 0.7, 0.7, 1.0, &light_g);
  light_spot_cutoff(90, &light_g);
  light_spot_exponent(8, &light_g);
  light_attenuation(1, 0, .0075, &light_g);

  setup_torch(&light_g, player_g);

  init_assets(seed, side_g, div_forest_g, forest_sparsity_g, forest_density_g, lod_g, ktree_density_g, ktree_depth_g);
}

void launch_window(int argc, char *argv[]){
  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH);

  glutInitWindowSize(LARGEUR_FEN, HAUTEUR_FEN);
  glutInitWindowPosition(50, 50);

  glutCreateWindow("Fraude.");
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  glutDisplayFunc(affichage);

  glutIdleFunc(idle);

  /* Controle manette */
  if (fps_g){
    glutJoystickFunc(joystick_player, 50);
  } else {
    glutJoystickFunc(joystick_god, 50);
  }

  glutReshapeFunc(reshape);

  glutMainLoop();
}