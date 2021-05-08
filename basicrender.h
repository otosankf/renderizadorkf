/*  The MIT License (MIT)
*
*   Copyright (c) 2021 [Pierre Alexis Carriel Monroy](https://github.com/otosankf)
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy
*   of this software and associated documentation files (the "Software"), to deal
*   in the Software without restriction, including without limitation the rights
*   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*   copies of the Software, and to permit persons to whom the Software is
*   furnished to do so, subject to the following conditions:
*
*   The above copyright notice and this permission notice shall be included in all
*   copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*   SOFTWARE.
*
*   [MIT License](http://opensource.org/licenses/MIT).
*/

#ifndef BASICRENDER_H
#define BASICRENDER_H 1

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif
#include <inttypes.h>
#include <math.h>

/*******************
*  data types
*******************/
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
  double w, h;
  unsigned int wpx, hpx;
} Camera;

/*******************
*  math functions
*******************/
double dotProd(double *u, double *v);
double magnitude(double *v);
void crossProd(double *n, double *u, double *v);
void normal(double *n,  double *a, double *b, double *c);
void unitV(double *v);
void uvV(double *uv, double *u, double *v);

/**
 * calculate normal vector from 3 points.
 *
 * calculate normal vector from AB✕AC from 3 points (a, b and c param),
 * using double arrays[3]. Point (v[0], v[1], v[2]).
 *
 * @param normal vector result
 * @param A point for AB and AC cectors
 * @param B point for AB vector
 * @param C point for AC vector
 * @return void
 */
void normal(double *n,  double *a, double *b, double *c) {

  double ab[3], ac[3];

  uvV(ab, a, b);
  uvV(ac, a, c);

  crossProd(n, ab, ac);
};

/**
 * Calculate cross product.
 *
 * calculate cross product vector from U✕V,
 * using double arrays[3]. vector (v[0], v[1], v[2]).
 *
 * @param cross product result
 * @param U vector for U✕V
 * @param V vector  for U✕V
 * @return void
 */
void crossProd(double *n, double *u, double *v) {

  n[0] = u[1] * v[2] - u[2] * v[1];
  n[1] = u[2] * v[0] - u[0] * v[2];
  n[2] = u[0] * v[1] - u[1] * v[0];
};

/**
 * Calculate dot product.
 *
 * calculate dot product escalar from U•V,
 * using double arrays[3]. vector (v[0], v[1], v[2]).
 *
 * @param U vector for U•V
 * @param V vector  for U•V
 * @return dot product result
 */
double dotProd(double *u, double *v) {
  return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
}

/**
 * Calculate the vector magnitude.
 *
 * calculate the vector magnitude from V vector,
 * using double arrays[3]. vector (v[0], v[1], v[2]).
 *
 * @param V vector
 * @return  V vector magnitude result
 */
double magnitude(double *v) {
  return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

/**
 * Calculate and override the unit vector for itself .
 *
 * calculate  and override the vector magnitude from V vector,
 * using double arrays[3]. vector (v[0], v[1], v[2]).
 *
 * @param V vector
 * @return  V vector magnitude result
 */
void unitV(double *v) {
  double m;
  m = magnitude(v);
  v[0] = v[0] / m;
  v[1] = v[1] / m;
  v[2] = v[2] / m;
}

/**
 * Calculate UV vector from U and V vector.
 *
 * Calculate UV vector from U and V vector  (V-U),
 * using double arrays[3]. vector (v[0], v[1], v[2]).
 *
 * @param UV vector result
 * @param U vector for V-U
 * @param V vector  for V-U
 * @return void
 */
void uvV(double *uv, double *u, double *v) {
  uv[0] = v[0] - u[0];
  uv[1] = v[1] - u[1];
  uv[2] = v[2] - u[2];
}
/**
 * Calculate the determinant  from 3x3 matrix.
 *
 * Calculate the determinant  from ABC 3x3 matrix,
 * using double arrays[3]. vector (v[0], v[1], v[2]).
 *
 * @param A vector
 * @param B vector for V-U
 * @param C vector  for V-U
 * @return determinant scalar result
 */
double determinant(double *a, double *b, double *c) {
  return (a[0] * b[1] * c[2] + b[0] * c[1] * a[2] + c[0] * a[1] * b[2]) - (a[0] * c[1] * b[2] + b[0] * a[1] * c[2] + c[0] * b[1] * a[2]);
}

/********************
 *  render function
 *******************/

 /**
  * Calculate t* value on parametric function r(t)
  *
  * Calculate t* value on parametric function r(t)= (1-t)*C + t*P:
  * where r(t*) ∩ ABC plane : ABC triangle
  * using double arrays[3], point (v[0], v[1], v[2]), and Triangle datatype.
  *
  * @param Triangle datatype
  * @param r(0), C value point
  * @param r(1), P value point
  * @return t* value, where r(t*) ∩ ABC plane
  */
double findt (Triangle * face, double * t0, double * t1 ) {
  return (face->a[0] * face->normal[0] + face->a[1] * face->normal[1] + face->a[2] * face->normal[2] - face->normal[0] * t0[0] - face->normal[1] * t0[1] - face->normal[2] * t0[2]) / (face->normal[0] * t1[0] + face->normal[1] * t1[1] + face->normal[2] * t1[2]  - face->normal[0] * t0[0] - face->normal[1] * t0[1] - face->normal[2] * t0[2]);
}

/**
 * Calculate I point on parametric function r(t)
 *
 * Calculate I point on parametric function r(t)= (1-t)*C + t*P,
 * using double arrays[3], point (v[0], v[1], v[2]).
 *
 * @param I result
 * @param r(0), C value point
 * @param r(1), P value point
 * @return void
 */
void findI(double * I, double t, double * C, double * P) {
  I[0] = (1-t) * C[0] + t * P[0];
  I[1] = (1-t) * C[1] + t * P[1];
  I[2] = (1-t) * C[2] + t * P[2];
}

#endif
