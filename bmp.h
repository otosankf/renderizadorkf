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

#ifndef BMP_H_INCLUDED
#define BMP_H_INCLUDED
#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif
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

#pragma pack (1)

typedef struct {
  uint16_t headerField: 16;
  uint32_t sizeFile: 32;
  uint16_t reserved1: 16;
  uint16_t reserved2: 16;
  uint32_t offset: 32;
}BitMapFileHeader;

typedef struct {
   uint32_t sizeHeader: 32;
   uint32_t whidth: 32;
   uint32_t heigth: 32;
   uint16_t numPlanes: 16;
   uint16_t bpp: 16;
   uint32_t compression: 32;
   uint32_t imageSize: 32;
   uint32_t horizontalRes: 32;
   uint32_t verticalRes: 32;
   uint32_t numColorPalette: 32;
   uint32_t numImportantColors: 32;
} BitMapInfoHeader;

typedef struct {
   uint32_t sizeHeader: 32;
   uint16_t whidth: 16;
   uint16_t heigth: 16;
   uint16_t numPlanes: 16;
   uint16_t bpp: 16;
} BitMapCoreHeader;

typedef struct {
   BitMapFileHeader fileHeader;
   BitMapInfoHeader infoHeader;
   //void* imagen;
} Bmp;

typedef struct {
  unsigned char b;
  unsigned char g;
  unsigned char r;
} Pixel;

#endif // BMP_H_INCLUDED
