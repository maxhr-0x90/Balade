#include "../inc/graphe.h"
#include "../inc/alloc.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

edge edge_init(int ext_a, int ext_b, float weight){
  edge e = safe_alloc(sizeof(struct edge_s));

  e->ext_a = ext_a;
  e->ext_b = ext_b;
  e->weight = weight;

  return e;
}

void edge_free(edge e){
  safe_free(e);
}

coord coord_init(float x, float y){
  coord c = safe_alloc(sizeof(struct coord_s));

  c->x = x;
  c->y = y;

  return c;
}

graph graph_init(array coords){
  graph g = safe_alloc(sizeof(struct graph_s));

  g->nb_vertices = array_size(coords);
  g->coords = coords;
  g->nb_edges = 0;
  
  return g;
}


int *init_partition(int nb) {
  int *p;
  int i;

  p = safe_alloc(nb*sizeof(int));
  for (i = 0; i < nb; i++) 
    p[i] = i;
  return p;
}

void fusion(int *p, int nb_edges, int a, int b) {
  int i, comp_b;

  comp_b = p[b];
  for (i=0; i < nb_edges; i++) {
    if (p[i] == comp_b) { p[i] = p[a]; }
  }
}

int same_class(int *p, int a, int b) {
  return (p[a] == p[b]);
}


int compare_edges(const void *a, const void *b) {
  double wa = (*((edge *) a))->weight;
  double wb = (*((edge *) b))->weight;
  
  return (int) (wa - wb);
}

double weight(int u, int v, graph g) {
  coord uc = array_get(u, g->coords);
  coord vc = array_get(v, g->coords);
  return hypotf(uc->x - vc->x, uc->y - vc->y);
}


void generate_edges(graph g) {
  g->nb_edges = (g->nb_vertices * (g->nb_vertices - 1)) / 2;
  g->edges = array_init(g->nb_edges);
  
  for (int i = 0; i < g->nb_vertices; i++){
    for (int j = i + 1; j < g->nb_vertices; j++){
      array_add(edge_init(i, j, weight(i, j, g)), g->edges);
    }
  }
}


graph generate_MST(array coords) {
  graph g = graph_init(coords);
  graph mst = graph_init(coords);
  edge e;

  // Algorithme de Kruskal

  generate_edges(g);
  int *partition = init_partition(g->nb_edges);
  
  mst->nb_edges = mst->nb_vertices - 1;
  mst->edges = array_init(mst->nb_edges); 

  qsort(g->edges->arr, g->nb_edges, sizeof(edge), compare_edges);

  for (int i = 0; i < g->nb_edges; i++){
    e = array_get(i, g->edges);
    if (!same_class(partition, e->ext_a, e->ext_b)) {
      array_add(e, mst->edges);
      fusion(partition, g->nb_edges, e->ext_a, e->ext_b);
    } else {
      edge_free(e);
    }
  }

  array_free(g->edges);
  safe_free(g);
  safe_free(partition);
  return mst;
}

void print_graph(graph g){
  printf("Vertices:\n");
  for (int i = 0; i < g->nb_vertices; i++){
    coord c = array_get(i, g->coords);
    printf("n°%d -> x: %f, y: %f\n", i, c->x, c->y);
  }
  
  printf("\nEdges:\n");
  for (int i = 0; i < g->nb_edges; i++){
    edge e = array_get(i, g->edges);
    printf("%d <-> %d : %f\n", e->ext_a, e->ext_b, e->weight);
  }
}