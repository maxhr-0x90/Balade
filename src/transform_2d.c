#include "../inc/transform_2d.h"

#include "../inc/vector.h"

#include <string.h>
#include <math.h>
#include <stdio.h>

void trans2d_id(trans_2d mat){
  memset(mat, 0, sizeof (trans_2d));
  mat[0] = 1;
  mat[4] = 1;
  mat[8] = 1;
}

// comb = cur * add (car Matrice à colonne prioritaire)
void trans2d_comb(trans_2d add, trans_2d cur, trans_2d comb){
  // 1ere colonne
  comb[0] = cur[0] * add[0] + cur[3] * add[1] + cur[6] * add[2];
  comb[1] = cur[1] * add[0] + cur[4] * add[1] + cur[7] * add[2];
  comb[2] = cur[2] * add[0] + cur[5] * add[1] + cur[8] * add[2];

  // 2eme colonne
  comb[3] = cur[0] * add[3] + cur[3] * add[4] + cur[6] * add[5];
  comb[4] = cur[1] * add[3] + cur[4] * add[4] + cur[7] * add[5];
  comb[5] = cur[2] * add[3] + cur[5] * add[4] + cur[8] * add[5];

  // 3eme colonne
  comb[6] = cur[0] * add[6] + cur[3] * add[7] + cur[6] * add[8];
  comb[7] = cur[1] * add[6] + cur[4] * add[7] + cur[7] * add[8];
  comb[8] = cur[2] * add[6] + cur[5] * add[7] + cur[8] * add[8];
}

void trans2d_translate(GLfloat x, GLfloat y, trans_2d mat){
  trans_2d comb, trans;
  
  trans2d_id(trans);
  trans[6] = x;
  trans[7] = y;

  trans2d_comb(trans, mat, comb);
  memcpy(mat, comb, sizeof (trans_2d));
}

void trans2d_rotate(GLfloat angle, trans_2d mat){
  trans_2d comb, trans;

  GLfloat c = cosf(angle);
  GLfloat s = sinf(angle);

  trans2d_id(trans);
  trans[0] = c;
  trans[1] = -s;

  trans[3] = s;
  trans[4] = c;

  trans2d_comb(trans, mat, comb);
  memcpy(mat, comb, sizeof (trans_2d));
}

void trans2d_scale(GLfloat x, GLfloat y, trans_2d mat){
  trans_2d comb, trans;
  
  trans2d_id(trans);
  trans[0] = x;
  trans[4] = y;

  trans2d_comb(trans, mat, comb);
  memcpy(mat, comb, sizeof (trans_2d));
}

int inverse2d(trans_2d src, trans_2d dest){
  trans_2d inv;

  float det = src[0] * (src[4] * src[8] - src[5] * src[7]) -
              src[1] * (src[3] * src[8] - src[5] * src[6]) +
              src[2] * (src[3] * src[7] - src[4] * src[6]);

  if (det == 0){ return 0; }

  inv[0] =  (src[4] * src[8] - src[7] * src[5]) / det;
  inv[1] = -(src[1] * src[8] - src[7] * src[2]) / det;
  inv[2] =  (src[1] * src[5] - src[4] * src[2]) / det;

  inv[3] = -(src[3] * src[8] - src[6] * src[5]) / det;
  inv[4] =  (src[0] * src[8] - src[6] * src[2]) / det;
  inv[5] = -(src[0] * src[5] - src[3] * src[2]) / det;

  inv[6] =  (src[3] * src[7] - src[6] * src[4]) / det;
  inv[7] = -(src[0] * src[7] - src[6] * src[1]) / det;
  inv[8] =  (src[0] * src[4] - src[3] * src[1]) / det;

  memcpy(dest, inv, sizeof (trans_2d));

  return 1;
}

void aff_trans_2d(trans_2d tr){
  for (int i = 0; i < 3; i++){
    for (int j = 0; j < 3; j++){
      printf("%f\t", tr[j * 3 + i]);
    }
    printf("\n");
  }
}

void transformv2(trans_2d tr, vector2f vec){
  vector2f u = {
    tr[0] * vec[0] + tr[3] * vec[1] + tr[6],
    tr[1] * vec[0] + tr[4] * vec[1] + tr[7]
  };

  float w = tr[2] * vec[0] + tr[5] * vec[1] + tr[8];

  multv2(u, 1/w);

  vec2_cpy(u, vec);
}

void trans2d_to_3d(trans_2d in, trans_3d out){
  trans_id(out);

  out[0] = in[0];
  out[1] = in[1];
  out[3] = in[2];

  out[4] = in[3];
  out[5] = in[4];
  out[7] = in[5];

  out[12] = in[6];
  out[13] = in[7];
  out[15] = in[8];
}