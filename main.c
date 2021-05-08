// The MIT License (MIT)
//
// Copyright (c) 2021 Pierre Alexis Carriel Monroy
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// [MIT License](http://opensource.org/licenses/MIT).



#include <stdio.h>
#include <stdlib.h>

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include <inttypes.h>
#include <float.h>
#include "basicrender.h"
#include "bmp.h"
#include "endianess.h"

int main(int argc, char const *argv[]) {

  FILE *fIn;
  FILE *fOut;
  char section;
  unsigned int qty, vQty, fQty, kQty;
  Point3d *vRaw;
  Triangle *triangles;
  Pixel *colors;

  Camera camera;

  double vCamOj[3], vCamOi[3];
  double pInd[3] , cp[3], ijV[3], I[3];

  unsigned long int i, j, k;
  unsigned int auxA, auxB, auxC;
  double deltai, deltaj;
  double play, t;
  long int intersectedTriangle;
  double paramDist;
  double ponda, pondb, pondc;
  Pixel ** canvas;
  unsigned int padding = 0x00000000;
  unsigned short int lineMod;

  BitMapFileHeader fileHeader;
  BitMapInfoHeader infoHeader;

  /*
  *  read param
  */
  printf("inicio\n");
  if(argc!=3) {
    printf("Uso: %s fich_entrada fich_salida\n", argv[0]);
    return(0);
  }

  fIn = fopen(argv[1],"r");
  if(fIn==NULL) {
    printf("Error abriendo entrada\n");
    return(0);
  }

  fOut = fopen(argv[2], "wb");
  if(fOut==NULL) {
    printf("Error creando salida\n");
    fclose(fIn);
    return(0);
  }
  /*
  *  read file
  */
  /* read Vertex */
  fscanf(fIn, " %c %u", &section, &qty);
  if ((section != 'v') || section != 'v' ) {
    fclose(fIn);
    fclose(fOut);
  }
  vRaw = (Point3d*) malloc (qty * sizeof(Point3d));
  vQty = qty;
  for ( i = 0; i < qty; i++) {
    fscanf(fIn, " %Lf %Lf %Lf", &vRaw[i].p[0], &vRaw[i].p[1], &vRaw[i].p[2]);
  }

  /* read face (triangle) */
  fscanf(fIn, " %c %u", &section, &qty);
  triangles = (Triangle*) malloc (qty * sizeof(Triangle));
  fQty = qty;
  for ( i = 0; i < qty; i++) {

    fscanf(fIn, " %u %u %u %u", &auxA, &auxB, &auxC, &triangles[i].colorId);

    triangles[i].a[0] = vRaw[auxA].p[0];
    triangles[i].a[1] = vRaw[auxA].p[1];
    triangles[i].a[2] = vRaw[auxA].p[2];
    triangles[i].b[0] = vRaw[auxB].p[0];
    triangles[i].b[1] = vRaw[auxB].p[1];
    triangles[i].b[2] = vRaw[auxB].p[2];
    triangles[i].c[0] = vRaw[auxC].p[0];
    triangles[i].c[1] = vRaw[auxC].p[1];
    triangles[i].c[2] = vRaw[auxC].p[2];
  }

  /* read color */
  fscanf(fIn, " %c %u",&section, &qty);
  kQty = qty;
  colors = (Pixel*) malloc (qty * sizeof(Pixel));
  for ( i = 0; i < qty; i++) {
    fscanf(fIn, " %u %u %u" , &auxA, &auxB, &auxC);
    colors[i].r = auxA;
    colors[i].g = auxB;
    colors[i].b = auxC;
  }

  /* read camera */
  fscanf(fIn, " %c %u \n",&section, &qty);
  fscanf(fIn, " %Lf %Lf %Lf %Lf %Lf %Lf %Lf %Lf %Lf %Lf %Lf %lu %lu", &camera.c[0], &camera.c[1], &camera.c[2], &camera.d[0], &camera.d[1], &camera.d[2], &camera.o[0], &camera.o[1], &camera.o[2], &camera.w, &camera.h, &camera.wpx, &camera.hpx);

  fclose(fIn);
  /* end read */

  /***************************
  *  preparation for raytracer/render
  ****************************/
  /* calculate triangles normal and determinant */
  for (i = 0; i < fQty; i++) {
    normal(triangles[i].normal, triangles[i].a, triangles[i].b, triangles[i].c);
    unitV(triangles[i].normal);
    triangles[i].ds = determinant(triangles[i].a, triangles[i].b, triangles[i].c);
  }

  /* calculate canvas output 1:
  *  unit vector for x and y output image refence.
  */
  normal(vCamOj, camera.c, camera.d, camera.o);
  unitV(vCamOj);
  uvV(vCamOi, camera.c, camera.o);
  unitV(vCamOi);

  /* calculate canvas output 2:
  *  -calculate initial position pixel matrix for canvas output.
  *     Note: The initial position is relative to the bottom-left corner of the output image
  *  -calculate 3d separation delta. 2d pixel output equivalent.
  */
  deltai = (double) camera.h / camera.hpx;
  deltaj = (double) camera.w / camera.wpx;

  pInd[0] = camera.d[0] - (((double)(3 - (int)camera.wpx % 2))*0.5 + (double)(camera.wpx / 2)) * deltaj * vCamOj[0];
  pInd[1] = camera.d[1] - (((double)(3 - (int)camera.wpx % 2))*0.5 + (double)(camera.wpx / 2)) * deltaj * vCamOj[1];
  pInd[2] = camera.d[2] - (((double)(3 - (int)camera.wpx % 2))*0.5 + (double)(camera.wpx / 2)) * deltaj * vCamOj[2];

  pInd[0] = pInd[0] - (((double)(-1 + (int)camera.hpx % 2))*0.5 + (double)(camera.hpx / 2)) * deltai * vCamOi[0];
  pInd[1] = pInd[1] - (((double)(-1 + (int)camera.hpx % 2))*0.5 + (double)(camera.hpx / 2)) * deltai * vCamOi[1];
  pInd[2] = pInd[2] - (((double)(-1 + (int)camera.hpx % 2))*0.5 + (double)(camera.hpx / 2)) * deltai * vCamOi[2];

  canvas = (Pixel**) malloc(camera.hpx * sizeof(void*));

  for (i = 0; i < camera.hpx; i++) {

    canvas[i] = (Pixel*)malloc(camera.wpx * sizeof(Pixel));
  }

  /***************************
  *  raytracer/render
  ****************************/
  for (i = 0; i < camera.hpx; i++) {

    /* set vertical canvas position image (i) */
    ijV[0] = pInd[0] + deltai * vCamOi[0] * (double) i;
    ijV[1] = pInd[1] + deltai * vCamOi[1] * (double) i;
    ijV[2] = pInd[2] + deltai * vCamOi[2] * (double) i;

    for (j = 0; j < camera.wpx; j++) {
      /* set horizontal canvas position image (j) */
      ijV[0] = ijV[0] + deltaj * vCamOj[0];
      ijV[1] = ijV[1] + deltaj * vCamOj[1];
      ijV[2] = ijV[2] + deltaj * vCamOj[2];

      /* Raytracer */
      intersectedTriangle = -1;
      paramDist = 0;
      uvV(cp, camera.c, ijV);

      for ( k = 0; k < fQty; k++) {
        //is not parallel?
        play = dotProd(cp, triangles[k].normal);

        if (play) {
          /* calculate t parametric function*/
          t = findt(&triangles[k], camera.c, ijV);

          /* find  nearest t (parametric function) to canvas */
          if ((t < paramDist || intersectedTriangle == -1) && t >= 1) {
            /* find if I is inside triangle */
            findI(I, t, camera.c, ijV);

            ponda = determinant( I, triangles[k].b, triangles[k].c)/triangles[k].ds;
            pondb = determinant( triangles[k].a, I, triangles[k].c)/triangles[k].ds;
            pondc = determinant( triangles[k].a, triangles[k].b, I)/triangles[k].ds;

            if ((ponda >= 0 && pondb >= 0 && pondc >= 0) || (ponda < 0 && pondb < 0 && pondc < 0)) {
              /* set nearest face founded (triangle) intersected */
              paramDist = t;
              intersectedTriangle = k;
            }
          }
        }
      }//end K

      /*
      *  Render pixel on canvas
      */
      if (intersectedTriangle < 0) {
        /* default background index color */
        auxB = 0;
      }else {
        auxB = triangles[intersectedTriangle].colorId;
      }
      canvas[i][j].r = colors[auxB].r;
      canvas[i][j].g = colors[auxB].g;
      canvas[i][j].b = colors[auxB].b;
    }// end j
  }// end i
  /* end raytracer/render */

  /**************************
  *  print image file (bmp)
  ***************************/
  /* Padding for 4 byte alignment */
  lineMod = (camera.wpx *(4-3)) % 4;

  /* set headers file */
  fileHeader.headerField = reverse_bytes_16(BM_TYPE);
  fileHeader.sizeFile = (54 + camera.wpx * camera.hpx * 3 + lineMod * camera.hpx);
  fileHeader.reserved1 = 0;
  fileHeader.reserved2 = 0;
  fileHeader.offset = (54);

  infoHeader.sizeHeader = (40);
  infoHeader.whidth = (camera.wpx);
  infoHeader.heigth = (camera.hpx);
  infoHeader.numPlanes = (1);
  infoHeader.bpp = (24);
  infoHeader.compression = (BI_RGB);
  infoHeader.imageSize = (camera.wpx * camera.hpx + 2 * camera.hpx);
  infoHeader.horizontalRes = (2835);
  infoHeader. verticalRes = (2835);
  infoHeader.numColorPalette = 0;
  infoHeader.numImportantColors = 0;

  fwrite((unsigned char *)&fileHeader, 1, 14, fOut);
  fwrite((unsigned char *)&infoHeader, 1, 40, fOut);

  /* print canvas (pixel matrix) on file */
  for (i = 0; i < camera.hpx; i++) {
    fwrite (&canvas[i][0], sizeof(Pixel), camera.wpx, fOut);
    if (lineMod > 0) {
      fwrite (&padding, 1, lineMod, fOut);
    }
  }

  fclose(fOut);
  free(vRaw);
  free(triangles);
  for ( i = 0; i < kQty; i++) {
    free(&colors[i]);
  }
  free(colors);

  return 0;
}
