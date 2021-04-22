#include <stdio.h>
#include <stdlib.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <float.h>
#include "basicrender.h"
#include "bmp.h"



int main(int argc, char const *argv[]) {

  FILE *fIn;
  FILE *fOut;
  char section;
  unsigned int qty, vQty, fQty, kQty;
  Point3d *vRaw;
  Triangle *triangles;
  Pixel *colors;

  Camera camera;

  double vCamOj[3];
  double pInd[3] , cd[3], ijV[3], I[3];

  int i, j, k;
  unsigned int auxA, auxB, auxC;
  double deltai, deltaj;
  double play, t;
  long int intersectedTriangle, paramDist;
  double ponda, pondb, pondc;
  Pixel ** canvas;

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
    fscanf(fIn, "%Lf %Lf %Lf", &vRaw[i].p[0], &vRaw[i].p[1], &vRaw[i].p[2]);
  }

  /* read face (triangle) */
  fscanf(fIn, " %c %u", &section, &qty);
  triangles = (Triangle*) malloc (qty * sizeof(Triangle));
  fQty = qty;
  for ( i = 0; i < qty; i++) {
    fscanf(fIn, " %u %u %u", &auxA, &auxB, &auxC);
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
    fscanf(fIn, "%" SCNu8 "%" SCNu8 "%" SCNu8, &colors[i].r, &colors[i].g, &colors[i].b);
  }

  /* read camera */
  fscanf(fIn, " %c %u",&section, &qty);
  fscanf(fIn, " %Lf %Lf %Lf %Lf %Lf %Lf %Lf %Lf %Lf %u %u %u %u", &camera.c[0], &camera.c[1], &camera.c[2], &camera.d[0], &camera.d[1], &camera.d[2], &camera.o[0], &camera.o[1], &camera.o[2], &camera.w, &camera.h, &camera.wpx, &camera.hpx);

  fclose(fIn);

  /* calculate triangles normal and determinant */
  for (i = 0; i < vQty; i++) {
    normal(triangles[i].normal, triangles[i].a, triangles[i].b, triangles[i].c);
    triangles[k].ds = determinant(triangles[i].a, triangles[i].b, triangles[i].c);
  }

  /* calculate canvas output */
  normal(vCamOj, camera.c, camera.o, camera.d);
  unitV(vCamOj);

  deltai = (double) camera.h / camera.hpx;
  deltaj = (double) camera.w / camera.wpx;

  uvV(cd, camera.c, camera.d);

  pInd[0] = cd[0] - ((1 + camera.wpx % 2)*0.5 + (camera.wpx / 2)) * deltaj * vCamOj[0];
  pInd[1] = cd[1] - ((1 + camera.wpx % 2)*0.5 + (camera.wpx / 2)) * deltaj * vCamOj[1];
  pInd[2] = cd[2] - ((1 + camera.wpx % 2)*0.5 + (camera.wpx / 2)) * deltaj * vCamOj[2];

  pInd[0] = pInd[0] - ((1 + camera.hpx % 2)*0.5 + (camera.hpx / 2)) * deltai * camera.o[0];
  pInd[1] = pInd[1] - ((1 + camera.hpx % 2)*0.5 + (camera.hpx / 2)) * deltai * camera.o[1];
  pInd[2] = pInd[2] - ((1 + camera.hpx % 2)*0.5 + (camera.hpx / 2)) * deltai * camera.o[2];

  canvas = (Pixel**) malloc(camera.hpx * sizeof(Pixel*));
  for (i = 0; i < camera.hpx; i++) {

    canvas[i] = (Pixel*)malloc(camera.wpx * sizeof(Pixel));
  }

  for (i = 0; i < camera.hpx; i++) {
    ijV[0] = pInd[0] + deltai * camera.o[0] * i;
    ijV[1] = pInd[1] + deltai * camera.o[1] * i;
    ijV[2] = pInd[2] + deltai * camera.o[2] * i;

    intersectedTriangle = -1;
    paramDist = 0;
    for (j = 0; j < camera.wpx; j++) {
      ijV[0] = pInd[0] + deltaj * vCamOj[0];
      ijV[1] = pInd[1] + deltaj * vCamOj[1];
      ijV[2] = pInd[2] + deltaj * vCamOj[2];

      // raytracer
      for ( k = 0; k < fQty; k++) {
        //is parallel?
        play = dotProd(cd, triangles[k].normal);
        if (play) {
          // find t (parametric function)
          t = (triangles[k].a[0] * triangles[k].normal[0] + triangles[k].a[1] * triangles[k].normal[1] + triangles[k].a[2] * triangles[k].normal[2] - triangles[k].normal[0] * camera.c[0] - triangles[k].normal[1] * camera.c[1] - triangles[k].normal[2] * camera.c[2]) / (triangles[k].normal[0] * camera.d[0] + triangles[k].normal[1] * camera.d[1] + triangles[k].normal[2] * camera.d[2]  - triangles[k].normal[0] * camera.c[0] - triangles[k].normal[1] * camera.c[1] - triangles[k].normal[2] * camera.c[2]);
          if (t < paramDist || t == 0) {
            //I is on triangle?
            I[0] = (1-t)*camera.c[0] + t * camera.d[0];
            I[1] = (1-t)*camera.c[1] + t * camera.d[1];
            I[2] = (1-t)*camera.c[2] + t * camera.d[2];

            ponda = determinant( I, triangles[k].b, triangles[k].c)/triangles[k].ds;
            pondb = determinant( triangles[k].a, I, triangles[k].c)/triangles[k].ds;
            pondc = determinant( triangles[k].a, triangles[k].b, I)/triangles[k].ds;

            if (ponda >= 0 && pondb >= 0 && pondc >= 0) {
              paramDist = t;
              intersectedTriangle = k;
            }
          }
        }
      }//end K
      // draw pixel
      if (intersectedTriangle < 0) {
        intersectedTriangle = 0;
      }
      canvas[i][j].r = colors[intersectedTriangle].r;
      canvas[i][j].g = colors[intersectedTriangle].g;
      canvas[i][j].b = colors[intersectedTriangle].b;
    }
  }
  //store bmp file
  fileHeader.headerField = BM_TYPE;
  fileHeader.sizeFile = sizeof(BitMapFileHeader) + sizeof(BitMapInfoHeader) + camera.wpx * camera.hpx + 2 * camera.hpx;
  fileHeader.reserved1 = 0;
  fileHeader.reserved2 = 0;
  fileHeader.offset = sizeof(BitMapFileHeader) + sizeof(BitMapInfoHeader);

  infoHeader.sizeHeader = sizeof(BitMapInfoHeader);
  infoHeader.whidth = camera.wpx;
  infoHeader.heigth = camera.hpx;
  infoHeader.numPlanes = 1;
  infoHeader.bpp = 24;
  infoHeader.compression = BI_RGB;
  infoHeader.imageSize = camera.wpx * camera.hpx + 2 * camera.hpx;
  infoHeader.horizontalRes = 2835;
  infoHeader. verticalRes = 2835;
  infoHeader.numColorPalette = 0;
  infoHeader.numImportantColors = 0;
  // short unsigned int headerField = BM_TYPE;
  // long int sizeFile = sizeof(BitMapFileHeader) + sizeof(BitMapInfoHeader) + camera.wpx * camera.hpx + 2 * camera.hpx;
  // short int reserved1 = 0;
  // short int reserved2 = 0;
  // long int offset = 54;
  //
  // fprintf(fOut,"%hb", BM_TYPE);
  // fwrite(&sizeFile, 4, 1, fOut);
  // fwrite(&reserved1, 2, 1, fOut);
  // fwrite(&reserved2, 2, 1, fOut);
  // fwrite(&offset, 4, 1, fOut);
  fwrite((unsigned char *)&fileHeader, 1, sizeof(BitMapFileHeader), fOut);
  fwrite((unsigned char *)&infoHeader, 1, sizeof(BitMapInfoHeader), fOut);


  for (i = 0; i < camera.hpx; i++) {
    fwrite (canvas[i], sizeof(uint8_t) * 3, camera.wpx, fOut);
    fwrite ("AB", sizeof(uint8_t) * 2, 1, fOut);
  }

  fclose(fOut);
  free(vRaw);
  free(triangles);
  free(colors);
  printf("todo OK");

  return 0;
}
