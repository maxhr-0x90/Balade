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
#include "../inc/tests.h"

#define LARGEUR_FEN 800
#define HAUTEUR_FEN 800

#define DOWN 1
#define RIGHT 2
#define LEFT 4
#define UP 8
#define L1 16
#define R1 32

int mouse_x_g, mouse_y_g;

int fps_g = 0;

player player_g;
float radius_g, phi_g, theta_g;

int forward_g, back_g, left_g, right_g, upward_g, downward_g;

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

  gluPerspective(fovz_g, ratio_g, 0.1, 100);
  if (fps_g){
    lookat_fps();
  } else {
    lookat_god();
  }

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  //test_triangle_AABB_intersection();
  test_octree();

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
  } else {
    glutPostRedisplay();
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
    player_g->pos[2] -= delta;
  } else if (upward_g){
    player_g->pos[2] += delta;
  }

  if (right_g | left_g | forward_g | back_g){
    normalizev2(player_g->speed);
    step(delta, player_g);
  }

  glutPostRedisplay();
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
    glutPostRedisplay();
  }
}

void joystick_god(unsigned int buttonMask, int x, int y, int z){
  if (buttonMask & L1){
    radius_g -= 1;
    radius_g = radius_g <= 0.5 ? 0.5 : radius_g;
  } else if (buttonMask & R1){
    radius_g += 1;
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
  }
}

void keydown(unsigned char key, int x, int y){
  switch (key){
  case 'f':
    fps_g = !fps_g;
     if (fps_g){
        glutJoystickFunc(joystick_player, 50);
      } else {
        glutJoystickFunc(joystick_god, 50);
      }
  
  default:
    break;
  }
  
}

void fenetre(int argc, char *argv[]){
  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH);

  glutInitWindowSize(LARGEUR_FEN, HAUTEUR_FEN);
  glutInitWindowPosition(50, 50);

  glutCreateWindow("Fraude.");
  glEnable(GL_DEPTH_TEST);

  glutDisplayFunc(affichage);

  glutIdleFunc(idle);

  /* Controle manette */
  if (fps_g){
    glutJoystickFunc(joystick_player, 50);
  } else {
    glutJoystickFunc(joystick_god, 50);
  }

  glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
  glutKeyboardFunc(keydown);

  glutReshapeFunc(reshape);

  glutMainLoop();
}

void usage(char *nom_prog){
  printf(
    "Usage: %s [-bifmh]\n"
    "Options:\n"
    "\t-b : affiche une boule\n"
    "\t-i : affiche l'intersection entre 2 boules\n"
    "\t-f : rend l'affichage des volumes filaire (boule et intersection)\n"
    "\t-m : affiche le modèle de la maison\n"
    "\nNB: Le scroll permet de zoomer et dezoomer\n",
    nom_prog
  );
  exit(-1);
}

int main(int argc, char *argv[]){
  radius_g = 10.0f;
  phi_g = 0;
  theta_g = 0;

  player_g = player_init();
  player_set_pos(-10, 0, 2, player_g);

  forward_g = 0;
  back_g = 0;
  left_g = 0;
  right_g = 0;
  downward_g = 0;
  upward_g = 0;

  init_test_octree();

  int opt;
  while((opt = getopt(argc, argv, "h")) != -1) { 
    switch(opt) { 
      case 'h': usage(argv[0]); break; 
      case '?': fprintf(stderr, "Err: Option non reconnue\n"); usage(argv[0]); break; 
    } 
  } 

  fenetre(argc, argv);

  return 0;
}
