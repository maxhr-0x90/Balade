#include "../inc/set.h"
#include "../inc/alloc.h"

set set_init(int size){
  set s = safe_alloc(sizeof (struct set_s));
  
  size = (size / sizeof(int)) * sizeof(int);

  s->set = safe_alloc(size);
  s->cap = size;

  return s;
}

void set_free(set s){
  if (s == NULL){ return; }

  safe_free(s->set);
  safe_free(s);
}

void set_add(int i, set s){
  if (s == NULL || i < 0 || i >= s->cap){ return; }

  s->set[i / sizeof(int)] |= 1 << (i % sizeof(int));
}

void set_rem(int i, set s){
  if (s == NULL || i < 0 || i >= s->cap){ return; }

  s->set[i / sizeof(int)] &= ~(1 << (i % sizeof(int)));
}

int set_has(int i, set s){
  if (s == NULL || i < 0 || i >= s->cap){ return 0; }

  return ((s->set[i / sizeof(int)] & ~(1 << (i % sizeof(int)))) != 0);
}

int set_is_empty(set s){
  if (s == NULL){ return 1; }

  int or = 0;
  for (int i = 0; i < (s->cap / sizeof(int)); i++){ or |= s->set[i]; }
  
  return (or == 0);
}

int hamming_weight(int x){
  //types and constants used in the functions below
  //uint64_t is an unsigned 64-bit integer variable type (defined in C99 version of C language)
  const int m1  = 0x55555555; //binary: 0101...
  const int m2  = 0x33333333; //binary: 00110011..
  const int m4  = 0x0f0f0f0f; //binary:  4 zeros,  4 ones ...
  const int m8  = 0x00ff00ff; //binary:  8 zeros,  8 ones ...
  const int m16 = 0x0000ffff; //binary: 16 zeros, 16 ones

  //This is a naive implementation, shown for comparison,
  //and to help in understanding the better functions.
  //This algorithm uses 24 arithmetic operations (shift, add, and).
  x = (x & m1 ) + ((x >>  1) & m1 ); //put count of each  2 bits into those  2 bits 
  x = (x & m2 ) + ((x >>  2) & m2 ); //put count of each  4 bits into those  4 bits 
  x = (x & m4 ) + ((x >>  4) & m4 ); //put count of each  8 bits into those  8 bits 
  x = (x & m8 ) + ((x >>  8) & m8 ); //put count of each 16 bits into those 16 bits 
  x = (x & m16) + ((x >> 16) & m16); //put count of each 32 bits into those 32 bits 

  return x;
}

int set_size(set s){
  if (s == NULL){ return -1; }

  int size = 0;
  for (int i = 0; i < (s->cap / sizeof(int)); i++){ 
    size += hamming_weight(s->set[i]); 
  }
  
  return size;
}

void set_union(set a, set b, set out){
  if (a == NULL || b == NULL || out == NULL || a->cap != b->cap || b->cap != out->cap){ return; }

  for (int i = 0; i < (a->cap / sizeof(int)); i++){ 
    out->set[i] = a->set[i] | b->set[i]; 
  }
}

void set_inter(set a, set b, set out){
  if (a == NULL || b == NULL || out == NULL || a->cap != b->cap || b->cap != out->cap){ return; }

  for (int i = 0; i < (a->cap / sizeof(int)); i++){ 
    out->set[i] = a->set[i] & b->set[i]; 
  }
}