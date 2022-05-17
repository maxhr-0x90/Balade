#include "../inc/transform_3d.h"

#include "../inc/vector.h"

#include <string.h>
#include <math.h>
#include <stdio.h>

void trans_id(trans_3d mat){
  memset(mat, 0, sizeof (trans_3d));
  mat[0] = 1;
  mat[5] = 1;
  mat[10] = 1;
  mat[15] = 1;
}

// comb = cur * add (car Matrice à colonne prioritaire)
void trans_comb(trans_3d add, trans_3d cur, trans_3d comb){
  // 1ere colonne
  comb[0]  = add[0] * cur[0] + add[1] * cur[4] + add[2] * cur[8] + add[3] * cur[12];
  comb[1]  = add[0] * cur[1] + add[1] * cur[5] + add[2] * cur[9] + add[3] * cur[13];
  comb[2]  = add[0] * cur[2] + add[1] * cur[6] + add[2] * cur[10] + add[3] * cur[14];
  comb[3]  = add[0] * cur[3] + add[1] * cur[7] + add[2] * cur[11] + add[3] * cur[15];

  // 2eme colonne
  comb[4]  = add[4] * cur[0] + add[5] * cur[4] + add[6] * cur[8] + add[7] * cur[12];
  comb[5]  = add[4] * cur[1] + add[5] * cur[5] + add[6] * cur[9] + add[7] * cur[13];
  comb[6]  = add[4] * cur[2] + add[5] * cur[6] + add[6] * cur[10] + add[7] * cur[14];
  comb[7]  = add[4] * cur[3] + add[5] * cur[7] + add[6] * cur[11] + add[7] * cur[15];

  // 3eme colonne
  comb[8]  = add[8] * cur[0] + add[9] * cur[4] + add[10] * cur[8] + add[11] * cur[12];
  comb[9]  = add[8] * cur[1] + add[9] * cur[5] + add[10] * cur[9] + add[11] * cur[13];
  comb[10] = add[8] * cur[2] + add[9] * cur[6] + add[10] * cur[10] + add[11] * cur[14];
  comb[11] = add[8] * cur[3] + add[9] * cur[7] + add[10] * cur[11] + add[11] * cur[15];

  // 4eme colonne
  comb[12] = add[12] * cur[0] + add[13] * cur[4] + add[14] * cur[8] + add[15] * cur[12];
  comb[13] = add[12] * cur[1] + add[13] * cur[5] + add[14] * cur[9] + add[15] * cur[13];
  comb[14] = add[12] * cur[2] + add[13] * cur[6] + add[14] * cur[10] + add[15] * cur[14];
  comb[15] = add[12] * cur[3] + add[13] * cur[7] + add[14] * cur[11] + add[15] * cur[15];
}

void trans_translate(GLfloat x, GLfloat y, GLfloat z, trans_3d mat){
  trans_3d comb, trans;
  
  trans_id(trans);
  trans[12] = x;
  trans[13] = y;
  trans[14] = z;

  trans_comb(trans, mat, comb);
  memcpy(mat, comb, sizeof (trans_3d));
}

void trans_rotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z, trans_3d mat){
  trans_3d comb, trans;

  vector3f vec = {x, y, z};
  normalizev3(vec);

  GLfloat c = cosf(angle);
  GLfloat s = sinf(angle);
  
  // Rodrigues' Rotation Formula
  trans_id(trans);
  trans[0] = c + x * x * (1 - c);
  trans[1] = z * s + x * y * (1 - c);
  trans[2] = -y * s + x * z * (1 - c);

  trans[4] = x * y * (1 - c) - z * s;
  trans[5] = c + y * y * (1 - c);
  trans[6] = x * s + y * z * (1 - c);

  trans[8] = y * s + x * z * (1 - c);
  trans[9] = -x * s + y * z * (1 - c);
  trans[10] = c + z * z * (1 - c);

  trans_comb(trans, mat, comb);
  memcpy(mat, comb, sizeof (trans_3d));
}

void trans_scale(GLfloat x, GLfloat y, GLfloat z, trans_3d mat){
  trans_3d comb, trans;
  
  trans_id(trans);
  trans[0] = x;
  trans[5] = y;
  trans[10] = z;

  trans_comb(trans, mat, comb);
  memcpy(mat, comb, sizeof (trans_3d));
}

int inverse(trans_3d src, trans_3d dest){
  double inv[16], det;
    int i;

    inv[0] = src[5]  * src[10] * src[15] - 
             src[5]  * src[11] * src[14] - 
             src[9]  * src[6]  * src[15] + 
             src[9]  * src[7]  * src[14] +
             src[13] * src[6]  * src[11] - 
             src[13] * src[7]  * src[10];

    inv[4] = -src[4]  * src[10] * src[15] + 
              src[4]  * src[11] * src[14] + 
              src[8]  * src[6]  * src[15] - 
              src[8]  * src[7]  * src[14] - 
              src[12] * src[6]  * src[11] + 
              src[12] * src[7]  * src[10];

    inv[8] = src[4]  * src[9] * src[15] - 
             src[4]  * src[11] * src[13] - 
             src[8]  * src[5] * src[15] + 
             src[8]  * src[7] * src[13] + 
             src[12] * src[5] * src[11] - 
             src[12] * src[7] * src[9];

    inv[12] = -src[4]  * src[9] * src[14] + 
               src[4]  * src[10] * src[13] +
               src[8]  * src[5] * src[14] - 
               src[8]  * src[6] * src[13] - 
               src[12] * src[5] * src[10] + 
               src[12] * src[6] * src[9];

    inv[1] = -src[1]  * src[10] * src[15] + 
              src[1]  * src[11] * src[14] + 
              src[9]  * src[2] * src[15] - 
              src[9]  * src[3] * src[14] - 
              src[13] * src[2] * src[11] + 
              src[13] * src[3] * src[10];

    inv[5] = src[0]  * src[10] * src[15] - 
             src[0]  * src[11] * src[14] - 
             src[8]  * src[2] * src[15] + 
             src[8]  * src[3] * src[14] + 
             src[12] * src[2] * src[11] - 
             src[12] * src[3] * src[10];

    inv[9] = -src[0]  * src[9] * src[15] + 
              src[0]  * src[11] * src[13] + 
              src[8]  * src[1] * src[15] - 
              src[8]  * src[3] * src[13] - 
              src[12] * src[1] * src[11] + 
              src[12] * src[3] * src[9];

    inv[13] = src[0]  * src[9] * src[14] - 
              src[0]  * src[10] * src[13] - 
              src[8]  * src[1] * src[14] + 
              src[8]  * src[2] * src[13] + 
              src[12] * src[1] * src[10] - 
              src[12] * src[2] * src[9];

    inv[2] = src[1]  * src[6] * src[15] - 
             src[1]  * src[7] * src[14] - 
             src[5]  * src[2] * src[15] + 
             src[5]  * src[3] * src[14] + 
             src[13] * src[2] * src[7] - 
             src[13] * src[3] * src[6];

    inv[6] = -src[0]  * src[6] * src[15] + 
              src[0]  * src[7] * src[14] + 
              src[4]  * src[2] * src[15] - 
              src[4]  * src[3] * src[14] - 
              src[12] * src[2] * src[7] + 
              src[12] * src[3] * src[6];

    inv[10] = src[0]  * src[5] * src[15] - 
              src[0]  * src[7] * src[13] - 
              src[4]  * src[1] * src[15] + 
              src[4]  * src[3] * src[13] + 
              src[12] * src[1] * src[7] - 
              src[12] * src[3] * src[5];

    inv[14] = -src[0]  * src[5] * src[14] + 
               src[0]  * src[6] * src[13] + 
               src[4]  * src[1] * src[14] - 
               src[4]  * src[2] * src[13] - 
               src[12] * src[1] * src[6] + 
               src[12] * src[2] * src[5];

    inv[3] = -src[1] * src[6] * src[11] + 
              src[1] * src[7] * src[10] + 
              src[5] * src[2] * src[11] - 
              src[5] * src[3] * src[10] - 
              src[9] * src[2] * src[7] + 
              src[9] * src[3] * src[6];

    inv[7] = src[0] * src[6] * src[11] - 
             src[0] * src[7] * src[10] - 
             src[4] * src[2] * src[11] + 
             src[4] * src[3] * src[10] + 
             src[8] * src[2] * src[7] - 
             src[8] * src[3] * src[6];

    inv[11] = -src[0] * src[5] * src[11] + 
               src[0] * src[7] * src[9] + 
               src[4] * src[1] * src[11] - 
               src[4] * src[3] * src[9] - 
               src[8] * src[1] * src[7] + 
               src[8] * src[3] * src[5];

    inv[15] = src[0] * src[5] * src[10] - 
              src[0] * src[6] * src[9] - 
              src[4] * src[1] * src[10] + 
              src[4] * src[2] * src[9] + 
              src[8] * src[1] * src[6] - 
              src[8] * src[2] * src[5];

    det = src[0] * inv[0] + src[1] * inv[4] + src[2] * inv[8] + src[3] * inv[12];

    if (det == 0)
        return 0;

    det = 1.0 / det;

    for (i = 0; i < 16; i++)
        dest[i] = inv[i] * det;

    return 1;
}

void aff_trans_3d(trans_3d tr){
  for (int i = 0; i < 4; i++){
    for (int j = 0; j < 4; j++){
      printf("%f\t", tr[j * 4 + i]);
    }
    printf("\n");
  }
}

void transformv3(trans_3d tr, vector3f vec){
  vector3f u = {
    tr[0] * vec[0] + tr[4] * vec[1] + tr[8] * vec[2] + tr[12],
    tr[1] * vec[0] + tr[5] * vec[1] + tr[9] * vec[2] + tr[13],
    tr[2] * vec[0] + tr[6] * vec[1] + tr[10] * vec[2] + tr[14]
  };

  float w = tr[3] * vec[0] + tr[7] * vec[1] + tr[11] * vec[2] + tr[15];

  multv3(u, 1/w);

  vec3_cpy(u, vec);
}