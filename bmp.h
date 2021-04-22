#ifndef BMP_H_INCLUDED
#define BMP_H_INCLUDED
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
//HEADER FIELD
#define BM_TYPE 0x424D //windows 3.1, 95 etc
#define BA_TYPE 0x4241 // OS/2 struct bitmap array
#define CI_TYPE 0x4349 // OS/2 struct color icon
#define CP_TYPE 0x4350 // OS/2 const color pinter
#define IC_TYPE 0x4943 // OS/2 struct icon
#define PT_TYPE 0x5054 // OS/2 pointer

//COMPRESSION
#define BI_RGB 0            //default
#define BI_RLE8 1           //Can be used only with 8-bit/pixel bitmaps
#define BI_RLE4 2           //Can be used only with 4-bit/pixel bitmaps
#define BI_BITFIELDS 3      //it field or Huffman 1D compression for BITMAPCOREHEADER2
#define BI_JPEG 4           //PEG or RLE-24 compression for BITMAPCOREHEADER2
#define BI_PNG 5            //The bitmap contains a PNG image
#define BI_ALPHABITFIELDS 6 //This value is valid in Windows CE .NET 4.0 and later.


typedef struct {
       short unsigned int headerField: 16;
       long int sizeFile: 32;
       short int reserved1: 16;
       short int reserved2: 16;
       long int offset: 32;

}__attribute__ ((packed)) BitMapFileHeader;

typedef struct {
   unsigned long int sizeHeader: 32;
   long int whidth: 32;
   long int heigth: 32;
   unsigned short int numPlanes: 16;
   unsigned short int bpp: 16;
   long unsigned int compression: 32;
   long unsigned int imageSize: 32;
   long int horizontalRes: 32;
   long int verticalRes: 32;
   long unsigned int numColorPalette: 32;
   long unsigned int numImportantColors: 32;

}__attribute__ ((packed)) BitMapInfoHeader;

typedef struct {
   long unsigned int sizeHeader: 32;
   unsigned short int whidth: 16;
   unsigned short int heigth: 16;
   unsigned short int numPlanes: 16;
   unsigned short int bpp: 16;
}__attribute__ ((packed)) BitMapCoreHeader;



typedef struct {
   BitMapFileHeader fileHeader;
   BitMapInfoHeader infoHeader;
   //void* imagen;
} Bmp;

typedef struct {
  uint8_t b;
  uint8_t g;
  uint8_t r;
}__attribute__ ((packed)) Pixel;

#endif // BMP_H_INCLUDED
