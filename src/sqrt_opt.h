#ifndef SQRT_OPT_H
#define SQRT_OPT_H

typedef float v4sf __attribute__ ((vector_size (16)));
typedef int v4si __attribute__ ((vector_size (16)));


template <size_t LOOPS = 2>
float sqrt1(float * a) {
  float root = 0;
  int *ai = reinterpret_cast<int *>(a);
  int *ri = reinterpret_cast<int *>(&root);

  *ri = (1 << 29) + (*ai >> 1) - (1 << 22) - 0x4C000;

  root = *reinterpret_cast<float *>(ri);

  for(unsigned int i=0; i < LOOPS; i++){
    root = 0.5 * (root + *a / root);
  }
  return root;
}

template <size_t LOOPS = 2>
void sqrt2(float * __restrict__ a, float * __restrict__ root) {
  int *ai = reinterpret_cast<int *>(a);
  int *ri = reinterpret_cast<int *>(root);

  *(ri + 0) = (1 << 29) + (*(ai + 0) >> 1) - (1 << 22) - 0x4C000;
  *(ri + 1) = (1 << 29) + (*(ai + 1) >> 1) - (1 << 22) - 0x4C000;
  *(ri + 2) = (1 << 29) + (*(ai + 2) >> 1) - (1 << 22) - 0x4C000;
  *(ri + 3) = (1 << 29) + (*(ai + 3) >> 1) - (1 << 22) - 0x4C000;

  root = reinterpret_cast<float *>(ri);

  for(unsigned int i=0; i < LOOPS; i++){
    *(root + 0) = 0.5 * (*(root + 0) + *(a + 0) / *(root + 0));
    *(root + 1) = 0.5 * (*(root + 1) + *(a + 1) / *(root + 1));
    *(root + 2) = 0.5 * (*(root + 2) + *(a + 2) / *(root + 2));
    *(root + 3) = 0.5 * (*(root + 3) + *(a + 3) / *(root + 3));
  }
}


template <size_t LOOPS = 2>
void v4sf_sqrt(v4sf *  __restrict__  a, v4sf *  __restrict__  root) {
  v4si *ai = reinterpret_cast<v4si *>(a);
  v4si *ri = reinterpret_cast<v4si *>(root);

  *(ri) = (1 << 29) + (*(ai) >> 1) - (1 << 22) - 0x4C000;
  
  root = reinterpret_cast<v4sf *>(ri);

  for(unsigned int i=0; i < LOOPS; i++){
    *(root) = 0.5 * (*root + *a / *root);
  }
}


// wrapper für v4sf_sqrt
template <size_t LOOPS = 2>
void sqrt3(float *  __restrict__  a, float *  __restrict__  root) {
  v4sf *as =  reinterpret_cast<v4sf *>(a);
  v4sf_sqrt<LOOPS>(as, reinterpret_cast<v4sf *>(root) );
}

#endif

