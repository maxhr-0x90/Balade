#include "../inc/gl_config.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void usage(char *nom_prog){
  printf(
    "Usage: %s [-S seed] [-s dim] [-n div] [-e sparsity] [-l lod] [-g group_size] [-p depth] [-v view]\n"
    "Options:\n"
    "\t-S : int : donne une graine [seed] pour la génération aléatoire\n"
    "\t-s : float : défini la taille du terrain: [dim] x [dim]\n"
    "\t-n : int : nombre de divisions [div] du terrain sur les 2 axes\n"
    "\t-e : int : éparpillement [sparsity] des arbres\n"
    "\t-l : int : nombre de divisions des divisions pour la création du modèle\n"
    "\t-g : int : nombre maximum d'instance dans un noeud d'arbre\n"
    "\t-d : int : profondeur maximale d'un  d'arbre\n"
    "\t-v : float : distance de rendu\n"
    "\nNB: Le scroll permet de zoomer et dezoomer\n",
    nom_prog
  );
  exit(-1);
}

int main(int argc, char *argv[]){
  int seed = -1;
  int opt;
  while((opt = getopt(argc, argv, "hS:s:n:e:l:g:p:v:")) != -1) { 
    switch(opt) { 
      case 'S': seed = atoi(optarg); break;
      case 's': side_g = atof(optarg); break;
      case 'n': div_forest_g = atoi(optarg); break;
      case 'e': forest_sparsity_g = atoi(optarg); break;
      case 'l': lod_g = atoi(optarg); break;
      case 'g': ktree_density_g = atoi(optarg); break;
      case 'p': ktree_depth_g = atoi(optarg); break;
      case 'v': sight_g = atof(optarg); break;
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
