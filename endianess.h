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

#ifndef ENDIANESS_H
#define ENDIANESs_H 1
  #ifdef __BYTE_ORDER__
    #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    #define reverse_bytes_32(data) ( ((data & 0xFF000000) >> 24) | ((data & 0x00FF0000) >> 8) | ((data & 0x0000FF00) << 8) | ((data & 0x000000FF) << 24) )
    #define reverse_bytes_16(data) ( ((data & 0xFF00) >> 8) | ((data & 0x00FF) << 8))

    #else
    #define reverse_bytes_32(data) (data)
    #define reverse_bytes_16(data) (data)
    #endif
  #else
    #if (1 == *(unsigned char *)&(const int){1})
    #define reverse_bytes_32(data) ( ((data & 0xFF000000) >> 24) | ((data & 0x00FF0000) >> 8) | ((data & 0x0000FF00) << 8) | ((data & 0x000000FF) << 24) )
    #define reverse_bytes_16(data) ( ((data & 0xFF00) >> 8) | ((data & 0x00FF) << 8))
    #else
    #define reverse_bytes_32(data) (data)
    #define reverse_bytes_16(data) (data)
    #endif
  #endif
#endif
