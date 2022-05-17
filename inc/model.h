#ifndef MODEL_H_
#define MODEL_H_

#include "gen_array.h"
#include "mesh.h"
#include "transform_3d.h"
#include "set.h"

typedef struct model_s* model;
struct model_s {
  array parts;
};

typedef struct mod_inst_s* mod_inst;
struct mod_inst_s{
  model mod;
  trans_3d mat;
};

typedef struct partial_s* partial;
struct partial_s{
  mod_inst parent;
  set mask;
};

// Initialise un modèle
model model_init(int parts);

// Libère l'espace occupé par le modèle
void model_free(model m);

// Ajoute une partie au modèle
void model_add_part(mesh part, model m);

// Renvoie une partie du modèle
mesh model_get_part(int i, model m);

mod_inst inst_init(model mod);

void inst_free(mod_inst inst);

partial partial_init(mod_inst inst);

void partial_free(partial part);

void partial_fuse(partial add, partial dest);

void print_partial(partial p);

#endif