

#include "grapher.h"

const s_color COLOR_WHITE       = {255, 255, 255};
const s_color COLOR_GRAY        = {128, 128, 128};
const s_color COLOR_LIGHT_GRAY  = {200, 200, 200};
const s_color COLOR_BLACK       = {  0,   0,   0};

const int BMP_BYTES_PER_PIXEL = 3; /// red, green, & blue
const int BMP_FILE_HEADER_SIZE = 14;
const int BMP_INFO_HEADER_SIZE = 40;

void bmp_draw_graph(s_tree* expression, const char* graph_path) {

    s_bmp_drawing_context context = {0};
    context.width = 600;
    context.height = 600;
    context.scale_x = 30;
    context.scale_y = 30;
    context.data = bmp_create_image(context.width, context.height);

    bmp_fill(&context, COLOR_WHITE);

    double view_x = - (double)context.width / 2;
    double view_y = - (double)context.height / 2;

    // Drawing vertical scale

    int dashes = (int) floor((double)context.height / 2 / context.scale_y);
    for(double y = -dashes * context.scale_y - view_x; y < context.height; y += context.scale_y) {
        bmp_draw_horizontal_line(&context, (int)round(y), 0, context.width, COLOR_LIGHT_GRAY);
    }

    dashes = (int) floor((double)context.width / 2 / context.scale_x);
    for(double x = -dashes * context.scale_x - view_y; x < context.height; x += context.scale_x) {
        bmp_draw_vertical_line(&context, (int)round(x), 0, context.height, COLOR_LIGHT_GRAY);
    }

    bmp_draw_vertical_line(&context, (int)round(-view_x), 0, context.height, COLOR_GRAY);
    bmp_draw_horizontal_line(&context, (int)round(-view_y), 0, context.width, COLOR_GRAY);

    const double** variables = calloc(TREE_CONSTANT_AMOUNT, sizeof(double));
    double x = 0;
    double y = 0;
    double old_y = 0;
    bool old_y_valid = false;
    variables[0] = &x;

    double max_step = 0.25;
    double min_step = 0.01;
    double step = max_step;

    for(double i = 0; i < context.width; i += step) {
        x = ((double)i + view_x) / context.scale_x;

        if(tree_evaluate(expression, variables, &y) == TREE_OK) {
            y *= context.scale_y;
            bmp_putpix(&context, (int)round(i), (int)round(y - view_y), COLOR_BLACK);
            if(old_y_valid) {
                if(fabs(y - old_y) > 1) {
                    if(step > min_step) {
                        i -= step;
                        step *= 0.5;
                    } else {
                        double y1 = old_y < y ? old_y : y;
                        double y2 = old_y < y ? y : old_y;

                        bmp_draw_vertical_line(&context, (int)round(i), (int)round(y1 - view_y), (int)round(y2 - view_y), COLOR_BLACK);
                        old_y_valid = true;
                        old_y = y;
                    }
                } else {
                    if(step < max_step) step *= 2;
                    else step = max_step;
                    old_y_valid = true;
                    old_y = y;
                }
            } else {
                old_y_valid = true;
                old_y = y;
            }
        } else {
            old_y_valid = false;
        }
    }

    bmp_write_image(context.data, context.height, context.width, graph_path);
}

void bmp_putpix(s_bmp_drawing_context* context, int x, int y, s_color color) {
    if(x < 0) return;
    if(x >= context->width) return;
    if(y < 0) return;
    if(y >= context->height) return;

    int texel = y * context->width * BMP_BYTES_PER_PIXEL + x * BMP_BYTES_PER_PIXEL;

    context->data[texel++] = color.red;
    context->data[texel++] = color.green;
    context->data[texel++] = color.blue;
}

void bmp_fill(s_bmp_drawing_context* context, s_color color) {
    int texel = context->width * context->height * BMP_BYTES_PER_PIXEL;
    unsigned char* image = context->data - 1;

    while(texel > 0) {
        image[texel--] = color.blue;
        image[texel--] = color.green;
        image[texel--] = color.red;
    }
}

void bmp_draw_vertical_line(s_bmp_drawing_context* context, int x, int y1, int y2, s_color color) {
    if(x < 0) return;
    if(x >= context->width) return;
    if(y1 < 0) y1 = 0;
    if(y1 >= context->height) return;
    if(y2 >= context->height) y2 = context->height - 1;
    if(y2 < 0) return;

    int texel = x * BMP_BYTES_PER_PIXEL + y1 * context->width * BMP_BYTES_PER_PIXEL;
    unsigned char* image = context->data;

    for(int j = y1; j < y2; j++) {
        image[texel++] = color.red;
        image[texel++] = color.green;
        image[texel++] = color.blue;
        texel += (context->width - 1) * BMP_BYTES_PER_PIXEL;
    }
}

void bmp_draw_horizontal_line(s_bmp_drawing_context* context, int y, int x1, int x2, s_color color) {
    if(y < 0) return;
    if(y >= context->height) return;
    if(x1 < 0) x1 = 0;
    if(x1 >= context->width) return;
    if(x2 >= context->width) x2 = context->width - 1;
    if(x2 < 0) return;

    int texel = y * context->width * BMP_BYTES_PER_PIXEL;
    unsigned char* image = context->data;

    for(int j = x1; j < x2; j++) {
        image[texel++] = color.red;
        image[texel++] = color.green;
        image[texel++] = color.blue;
    }
}

unsigned char* bmp_create_image(int width, int height) {
    return calloc(1, width * height * 3);
}

bool bmp_write_image(unsigned char* image, int height, int width, const char* filename) {
    int width_bytes = width * BMP_BYTES_PER_PIXEL;

    unsigned char padding[3] = {0, 0, 0};
    int padding_size = (4 - (width_bytes) % 4) % 4;

    int stride = (width_bytes) + padding_size;

    FILE* image_file = fopen(filename, "wb");
    if(!image_file) return false;

    bmp_write_file_header(height, stride, image_file);
    bmp_write_image_header(height, width, image_file);

    for (int i = 0; i < height; i++) {
        fwrite(image + (i * width_bytes), BMP_BYTES_PER_PIXEL, width, image_file);
        fwrite(padding, 1, padding_size, image_file);
    }

    fclose(image_file);
    return true;
}

void bmp_write_file_header(int height, int stride, FILE* file) {
    int fileSize = BMP_FILE_HEADER_SIZE + BMP_INFO_HEADER_SIZE + (stride * height);

    unsigned char fileHeader[] = {
            0,0,     /// signature
            0,0,0,0, /// image file size in bytes
            0,0,0,0, /// reserved
            0,0,0,0, /// start of pixel array
    };

    fileHeader[ 0] = (unsigned char)('B');
    fileHeader[ 1] = (unsigned char)('M');
    fileHeader[ 2] = (unsigned char)(fileSize      );
    fileHeader[ 3] = (unsigned char)(fileSize >>  8);
    fileHeader[ 4] = (unsigned char)(fileSize >> 16);
    fileHeader[ 5] = (unsigned char)(fileSize >> 24);
    fileHeader[10] = (unsigned char)(BMP_FILE_HEADER_SIZE + BMP_INFO_HEADER_SIZE);

    fwrite(fileHeader, 1, BMP_FILE_HEADER_SIZE, file);
}

void bmp_write_image_header(int height, int width, FILE* file) {
    unsigned char infoHeader[] = {
            0,0,0,0, /// header size
            0,0,0,0, /// image width
            0,0,0,0, /// image height
            0,0,     /// number of color planes
            0,0,     /// bits per pixel
            0,0,0,0, /// compression
            0,0,0,0, /// image size
            0,0,0,0, /// horizontal resolution
            0,0,0,0, /// vertical resolution
            0,0,0,0, /// colors in color table
            0,0,0,0, /// important color count
    };

    infoHeader[ 0] = (unsigned char)(BMP_INFO_HEADER_SIZE);
    infoHeader[ 4] = (unsigned char)(width      );
    infoHeader[ 5] = (unsigned char)(width >>  8);
    infoHeader[ 6] = (unsigned char)(width >> 16);
    infoHeader[ 7] = (unsigned char)(width >> 24);
    infoHeader[ 8] = (unsigned char)(height      );
    infoHeader[ 9] = (unsigned char)(height >>  8);
    infoHeader[10] = (unsigned char)(height >> 16);
    infoHeader[11] = (unsigned char)(height >> 24);
    infoHeader[12] = (unsigned char)(1);
    infoHeader[14] = (unsigned char)(BMP_BYTES_PER_PIXEL * 8);

    fwrite(infoHeader, 1, BMP_INFO_HEADER_SIZE, file);
}