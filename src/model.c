#include "../inc/model.h"
#include "../inc/alloc.h"

// Initialise un modèle
model model_init(int parts){
  model m = safe_alloc(sizeof(struct model_s));
  m->parts = array_init(parts);

  return m;
}

// Libère l'espace occupé par le modèle
void model_free(model m){
  if (m == NULL){ return; }

  array_free(m->parts);
  safe_free(m);
}

// Ajoute une partie au modèle
void model_add_part(mesh part, model m){
  if (m == NULL || part == NULL){ return; }

  array_add(part, m->parts);
}

// Renvoie une partie du modèle
mesh model_get_part(int i, model m){
  if (m == NULL){ return NULL; }

  return array_get(i, m->parts);
}

// Crée une copie du model
model model_copy(model src){
  if (src == NULL){ return NULL; }

  model cpy = safe_alloc(sizeof(struct model_s));
  cpy->parts = array_copy(mesh_copy, src->parts);

  return cpy;
}