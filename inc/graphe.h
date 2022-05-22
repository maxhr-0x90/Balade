/* Module gérant un graphe */

#ifndef GRAPHE_H_
#define GRAPHE_H_

#include "gen_array.h"
#include "vector.h"

// Structure d'une arête
typedef struct edge_s *edge;
struct edge_s{
  int ext_a, ext_b;
  float weight;
};

// Structure d'un coordonnée 2D
typedef struct coord_s *coord;
struct coord_s {
  float x, y;
};

// Structure d'un graphe
typedef struct graph_s *graph;
struct graph_s{
  int nb_vertices;
  array coords;

  int nb_edges; 
  array edges; 
};

// Initialise une coordonnée
coord coord_init(float x, float y);

// Crée un arbre couvrant de poids minimal 
graph generate_MST(array coords);

// Affiche un graphe
void print_graph(graph g);

#endif