#include <stdio.h>

#include "../inc/model_loader.h"

model load_model(const char *filename){
  int nb_part;
  FILE *file = fopen(filename, "r");
  if (file == NULL){
    fprintf(stderr, "fichier non trouvé\n");
    return NULL;
  }

  if (fscanf(file, "object %d\n", &nb_part) == EOF){ return NULL; }
  fprintf(stderr, "objet: nb parties: %d\n", nb_part);

  model mod = model_init(nb_part);

  int vdec = 1;
  int udec = 1;
  int ndec = 1;
  for (int p = 0; p < nb_part; p++){
    fprintf(stderr, "Chargement du materiau de %s\n", filename);
    material mat;
    fscanf(file, "Ns %f\n", &mat.shininess);
    fscanf(
      file, 
      "Ka %f %f %f\n", 
      &mat.ambient[0], &mat.ambient[1], &mat.ambient[2]
    );
    fscanf(
      file, 
      "Kd %f %f %f\n", 
      &mat.diffuse[0], &mat.diffuse[1], &mat.diffuse[2]
    );
    fscanf(
      file, 
      "Ks %f %f %f\n", 
      &mat.specular[0], &mat.specular[1], &mat.specular[2]
    );
    fscanf(
      file, 
      "Ke %f %f %f\n", 
      &mat.emission[0], &mat.emission[1], &mat.emission[2]
    );

    fprintf(
      stderr, 
      "Matériau chargé:\n"
      "Shininess: %f\n"
      "Ambient: %f %f %f\n"
      "Diffuse: %f %f %f\n"
      "Specular: %f %f %f\n"
      "Emission: %f %f %f\n",
      mat.shininess,
      mat.ambient[0], mat.ambient[1], mat.ambient[2],
      mat.diffuse[0], mat.diffuse[1], mat.diffuse[2],
      mat.specular[0], mat.specular[1], mat.specular[2],
      mat.emission[0], mat.emission[1], mat.emission[2]
    );

    mat.ambient[3] = 1.0f;
    mat.diffuse[3] = 1.0f;
    mat.specular[3] = 1.0f;
    mat.emission[3] = 1.0f;

    int verts, norms, uvcoord, faces;

    fscanf(file, "vertices %d\n", &verts);
    fscanf(file, "normals %d\n", &norms);
    fscanf(file, "uvcoords %d\n", &uvcoord);
    fscanf(file, "faces %d\n", &faces);

    fprintf(
      stderr, 
      "\nvertex:%d, normales:%d, uvcoords:%d, faces:%d\n", 
      verts, norms, uvcoord, faces
    );
    
    mesh me = mesh_init(mat, faces, verts, norms, uvcoord);

    fprintf(stderr, "Chargement des vertexs de %s\n", filename);
    float x, y, z;
    for (int i = 0; i < verts; i++){
      fscanf(file, "v %f %f %f\n", &x, &z, &y);
      mesh_add_vert(vertex_init(x, y, z), me);
    }
    
    fprintf(stderr, "Chargement des normales de %s\n", filename);
    for (int i = 0; i < norms; i++){
      fscanf(file, "vn %f %f %f\n", &x, &z, &y);
      mesh_add_norm(normal_init(x, y, z), me);
    }

    fprintf(stderr, "Chargement des uvcoords de %s\n", filename);
    for (int i = 0; i < uvcoord; i++){
      fscanf(file, "vt %f %f\n", &x, &y);
      mesh_add_uvc(uvcoord_init(x, y), me);
    }

    fprintf(stderr, "Chargement des faces de %s\n", filename);
    for (int i = 0; i < faces; i++){
      int v0, v1, v2, u0, u1, u2, n;
      fscanf(
        file, "f %d/%d/%d %d/%d/%d %d/%d/%d\n", 
        &v0, &u0, &n, &v1, &u1, &n, &v2, &u2, &n);
      mesh_add_face(face_init(
        v0 - vdec, v1 - vdec, v2 - vdec, 
        u0 - udec, u1 - udec, u2 - udec, 
        n - ndec
      ), me);
    }

    model_add_part(me, mod);
    vdec += verts;
    udec += uvcoord;
    ndec += norms;
  }

  fclose(file);
  return mod;
}