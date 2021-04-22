#ifndef BASICRENDER_H
#define BASICRENDER_H 1

#include <math.h>

#define __STDC_FORMAT_MACROS
#include <inttypes.h>
//datatype
typedef struct {
  double a[3], b[3], c[3], normal[3];
  unsigned int colorId;
  double ds;
} Triangle;

typedef struct {
  double p[3];
} Point3d;

typedef struct {
  double c[3], d[3], o[3];
  unsigned int w, h, wpx, hpx;
} Camera;

void crossProd(double *n, double *u, double *v);
void normal(double *n,  double *a, double *b, double *c);
double magnitude(double *v);
void unitV(double *v);
void uvV(double *uv, double *u, double *v);

void normal(double *n,  double *a, double *b, double *c) {

  double ab[3], ac[3];

  ab[0] = b[0] - a[0];
  ab[1] = b[1] - a[1];
  ab[2] = b[2] - a[2];

  ac[0] = c[0] - a[0];
  ac[1] = c[1] - a[1];
  ac[2] = c[2] - a[2];

  crossProd(n, ab, ab);
};

void crossProd(double *n, double *u, double *v) {

  n[0] = u[1] * v[2] - u[2] * v[1];
  n[1] = u[2] * v[0] - u[0] * v[2];
  n[2] = u[0] * v[1] - u[1] * v[0];
};

double dotProd(double *u, double *v) {
  return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
}

double magnitude(double *v) {
  return sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}
void unitV(double *v) {
  double m;
  m = magnitude(v);
  v[0] = v[0] / m;
  v[1] = v[1] / m;
  v[2] = v[2] / m;
}

void uvV(double *uv, double *u, double *v) {
  uv[0] = v[0] - u[0];
  uv[1] = v[1] - u[1];
  uv[2] = v[2] - u[2];
}

double determinant(double *a, double *b, double *c) {
  return (a[0] * b[1] * c[2] + b[0] * c[1] * a[2] + c[0] * a[1] * b[2]) - (a[0] * c[1] * b[2] + b[0] * a[1] * c[2] + c[0] * b[1] * a[2]);
}


#endif
