#include "../inc/gl_config.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void usage(char *nom_prog){
  printf(
    "Usage: %s [-s seed] [-bifmh]\n"
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
  int seed = -1;
  int opt;
  while((opt = getopt(argc, argv, "h")) != -1) { 
    switch(opt) { 
      case 'h': usage(argv[0]); break; 
      case '?': fprintf(stderr, "Err: Option non reconnue\n"); usage(argv[0]); break; 
    } 
  } 

  if (seed != -1){
    init_config(seed);
  } else {
    init_config(69420);
  }
  
  
  launch_window(argc, argv);

  return 0;
}
