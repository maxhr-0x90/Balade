#include "../inc/model.h"
#include "../inc/alloc.h"

#include <stdio.h>

// Initialise un modèle
model model_init(int parts){
  model m = safe_alloc(sizeof(struct model_s));
  m->parts = array_init(parts);

  return m;
}

// Libère l'espace occupé par le modèle
void model_free(model m){
  if (m == NULL){ return; }

  for (int i = 0; i < array_size(m->parts); i++){
    mesh_free(array_get(i, m->parts));
  }
  
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

mod_inst inst_init(model mod){
  mod_inst inst = safe_alloc(sizeof(struct mod_inst_s));
  inst->mod = mod;
  trans_id(inst->mat);

  return inst;
}

void inst_free(mod_inst inst){
  if(inst == NULL){ return; }

  safe_free(inst);
}

partial partial_init(mod_inst inst){
  partial part = safe_alloc(sizeof(struct partial_s));
  part->parent = inst;
  part->mask = set_init(array_size(inst->mod->parts));

  return part;
}

void partial_free(partial part){
  if(part == NULL){ return; }

  set_free(part->mask);
  safe_free(part);
}

void partial_fuse(partial add, partial dest){
  if (add == NULL || dest == NULL){ return; }
  if (add->parent != dest->parent){ return; }

  set_union(add->mask, dest->mask, dest->mask);
}

void print_partial(partial p){
  printf("parent: %p\n", (void *)p->parent);
  printf("mask: ");

  for (int i = 0; i < array_size(p->parent->mod->parts); i++){
    printf("%d ", set_has(i, p->mask));
  }
  printf("\n");
}