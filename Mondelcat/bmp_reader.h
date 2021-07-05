
#ifndef MANDELBROT_BMP_READER_H
#define MANDELBROT_BMP_READER_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#pragma pack(push, 1)

typedef struct bitmap_file_header
{
    int16_t bfType;  //specifies the file type
    int32_t bfSize;  //specifies the size in bytes of the bitmap file
    int16_t bfReserved1;  //reserved; must be 0
    int16_t bfReserved2;  //reserved; must be 0
    int32_t bfOffBits;  //species the offset in bytes from the bitmapfileheader to the bitmap bits
} s_bitmap_file_header;

typedef struct bitmap_info_header
{
    int32_t biSize;  //specifies the number of bytes required by the struct
    int32_t biWidth;  //specifies width in pixels
    int32_t biHeight;  //species height in pixels
    int16_t biPlanes; //specifies the number of color planes, must be 1
    int16_t biBitCount; //specifies the number of bit per pixel
    int32_t biCompression;//spcifies the type of compression
    int32_t biSizeImage;  //size of image in bytes
    int32_t biXPelsPerMeter;  //number of pixels per meter in x axis
    int32_t biYPelsPerMeter;  //number of pixels per meter in y axis
    int32_t biClrUsed;  //number of colors used by th ebitmap
    int32_t biClrImportant;  //number of colors that are important
} s_bitmap_info_header;

#pragma pack(pop)

char* load_bitmap_file(char* filename, s_bitmap_info_header* header);
void bmp_flip_image_vertical(char* bitmapImage, s_bitmap_info_header* header);
void bmp_flip_image_horizontal(char* bitmapImage, s_bitmap_info_header* header);
long long buffer_size(long long width, long long height);

#endif //MANDELBROT_BMP_READER_H
