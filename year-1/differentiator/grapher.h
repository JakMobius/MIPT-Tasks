//
// Created by Темыч on 30.11.2020.
//

#ifndef DIFFERENTIATOR_GRAPHER_H
#define DIFFERENTIATOR_GRAPHER_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "tree.h"

typedef struct bmp_drawing_context {
    int width;
    int height;
    double scale_x; // pixels per 1 x
    double scale_y; // pixels per 1 y
    unsigned char* data;
} s_bmp_drawing_context;

typedef struct color {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} s_color;

extern const s_color COLOR_WHITE;
extern const s_color COLOR_GRAY;
extern const s_color COLOR_LIGHT_GRAY;
extern const s_color COLOR_BLACK;

void bmp_draw_graph(s_tree* expression, const char* graph_path);
unsigned char* bmp_create_image(int width, int height);
bool bmp_write_image(unsigned char* image, int height, int width, const char* filename);
void bmp_write_file_header(int height, int stride, FILE* file);
void bmp_write_image_header(int height, int width, FILE* file);
void bmp_fill(s_bmp_drawing_context* context, s_color color);

void bmp_putpix(s_bmp_drawing_context* context, int x, int y, s_color color);
void bmp_draw_vertical_line(s_bmp_drawing_context* context, int x, int y1, int y2, s_color color);
void bmp_draw_horizontal_line(s_bmp_drawing_context* context, int y, int x1, int x2, s_color color);

#endif //DIFFERENTIATOR_GRAPHER_H
