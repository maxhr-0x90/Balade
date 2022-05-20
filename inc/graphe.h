#ifndef GRAPHE_H_
#define GRAPHE_H_

#include "gen_array.h"
#include "vector.h"

typedef struct edge_s *edge;
struct edge_s{
  int ext_a, ext_b;
  float weight;
};

typedef struct coord_s *coord;
struct coord_s {
  float x, y;
};

typedef struct graph_s *graph;
struct graph_s{
  int nb_vertices;
  array coords;

  int nb_edges; 
  array edges; 
};

coord coord_init(float x, float y);

graph generate_MST(array coords);
void print_graph(graph g);

#endif