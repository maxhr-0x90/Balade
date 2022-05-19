#include "GL/gl.h"
#include "GL/glut.h"

#include <unistd.h>
#include <stdio.h>
#include <math.h>

#include "../inc/voxel.h"
#include "../inc/model.h"
#include "../inc/player.h"
#include "../inc/light.h"
#include "../inc/model_generator.h"
#include "../inc/asset_placement.h"
#include "../inc/model_loader.h"

#define LARGEUR_FEN 800
#define HAUTEUR_FEN 800

#define DOWN 1
#define RIGHT 2
#define LEFT 4
#define UP 8

int mouse_x_g, mouse_y_g;

int just_entered = 1;

player player_g;

model mod_g, mod2_g;

mod_inst inst_g, inst2_g;

light light_g;

int forward_g, back_g, left_g, right_g;

int wind_width_g = LARGEUR_FEN;
int wind_height_g = HAUTEUR_FEN;

GLdouble fovz_g = 67;
GLdouble ratio_g = 1;

void affichage(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(fovz_g, ratio_g, 0.1, 100);
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

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  place_light(light_g, GL_LIGHT0);

  place_model(inst_g);
  place_model(inst2_g);

  glFlush();
}

void entry(int state){
  just_entered = (state == GLUT_ENTERED);
}

void motion(int x, int y){
  if (just_entered){
    mouse_x_g = x;
    mouse_y_g = y;
    just_entered = 0;
    return;
  }

  int dx = x - mouse_x_g;
  int dy = y - mouse_y_g;

  float theta = (float)dx / wind_height_g * (fovz_g * ratio_g * M_PI / 180);
  float phi = (float)dy / wind_height_g * (fovz_g * M_PI / 180);

  mouse_x_g = x;
  mouse_y_g = y;

  player_set_cam(player_g->phi + phi, player_g->theta + theta, player_g);

  glutPostRedisplay();
}

void keydown(unsigned char key, int x, int y){
  switch (key){
  case 'w':
    forward_g = 1;
    back_g = 0;
    break;
  
  case 'a':
    left_g = 1;
    right_g = 0;
    break;

  case 's':
    forward_g = 0;
    back_g = 1;
    break;

  case 'd':
    left_g = 0;
    right_g = 1;
    break;
  
  default:
    break;
  }
  
}

void keyup(unsigned char key, int x, int y){
  switch (key){
  case 'w':
    forward_g = 0;
    break;
  
  case 'a':
    left_g = 0;
    break;

  case 's':
    back_g = 0;
    break;

  case 'd':
    right_g = 0;
    break;
  
  default:
    break;
  }
  
}

void reshape(int width, int height){
  wind_width_g = width;
  wind_height_g = height;
  ratio_g = (GLdouble)width / height;
  glViewport(0, 0, width, height);
}

void idle(){
  if (!(forward_g | back_g | left_g | right_g)){ return; }

  float delta = 0.005f;

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

  normalizev2(player_g->speed);
  multv2(player_g->speed, delta);
  step(player_g);
  setup_torch(&light_g, player_g);

  glutPostRedisplay();
}

void joystick(unsigned int buttonMask, int x, int y, int z){
  if (buttonMask & DOWN & ~UP){
    back_g = 1;
  } else {
    back_g = 0;
  }

  if (buttonMask & RIGHT & ~LEFT){
    right_g = 1;
  } else {
    right_g = 0;
  }

  if (buttonMask & LEFT & ~RIGHT){
    left_g = 1;
  } else {
    left_g = 0;
  }

  if (buttonMask & UP & ~DOWN){
    forward_g = 1;
  } else {
    forward_g = 0;
  }

  if (x != 0 || y != 0){
    float angluar_speed = 0.05;
    float theta = ((-x * 0.18) * M_PI / 180) * angluar_speed;
    float phi = ((y * 0.18) * M_PI / 180) * angluar_speed;
    player_set_cam(player_g->phi + phi, player_g->theta + theta, player_g);
    setup_torch(&light_g, player_g);
    glutPostRedisplay();
  }
}

void fenetre(int argc, char *argv[]){
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
  glutJoystickFunc(joystick, 50);

  /* Controles clavier - souris */
  // glutKeyboardFunc(keydown);
  // glutKeyboardUpFunc(keyup);

  //glutSetCursor(GLUT_CURSOR_CROSSHAIR);

  //glutPassiveMotionFunc(motion);

  //glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);

  glutReshapeFunc(reshape);

  glutEntryFunc(entry);

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
  player_g = player_init();
  player_set_pos(-10, 0, 2, player_g);

  forward_g = 0;
  back_g = 0;
  left_g = 0;
  right_g = 0;

  light_default(&light_g);
  light_ambient(1, 1, 1, 1, &light_g);
  light_specular(0.2, 0.2, 0.2, 1.0, &light_g);
  light_spot_cutoff(70, &light_g);

  setup_torch(&light_g, player_g);

  int opt;
  while((opt = getopt(argc, argv, "h")) != -1) { 
    switch(opt) { 
      case 'h': usage(argv[0]); break; 
      case '?': fprintf(stderr, "Err: Option non reconnue\n"); usage(argv[0]); break; 
    } 
  } 

  mod_g = ground_model(100, 100, 10, 10);
  mod2_g = load_model("models/pine.swag");

  inst_g = inst_init(mod_g);
  trans_rotate(M_PI_4, 0, 0, 1, inst_g->mat);

  inst2_g = inst_init(mod2_g);

  fenetre(argc, argv);

  return 0;
}
