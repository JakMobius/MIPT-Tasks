
#include "messed_drawer.h"

void messed_thing_drawer(char* framebuffer, long long fb_width, long long fb_height, double zoom, double camera_x, double camera_y) {

    double viewport_width = zoom * (double) fb_width;
    double viewport_height = zoom * (double) fb_height;

    double half_viewport_width = viewport_width / 2;
    double half_viewport_height = viewport_height / 2;

    double left_viewport_boundary = camera_x - half_viewport_width;
    double x_step = viewport_width / (double) fb_width;

    double sse_x_step = x_step * 2;

    double top_viewport_boundary = camera_y - half_viewport_height;
    double y_step = viewport_height / (double) fb_height;

    double fractal_y = top_viewport_boundary;
    long long viewport_y = fb_height - 1;

    double radius = 10;
    double square_radius = radius * radius;

    __m128 square_radius_sse = _mm_set1_pd(square_radius);
    __m128 two_sse = _mm_set1_pd(2.0);
    __m128 shift_mask = _mm_set_epi32(0, 0, 0, 63);

    char* target = framebuffer;

    for(; viewport_y >= 0; viewport_y--) {

        double fractal_x = left_viewport_boundary;
        long long viewport_x = 0;

        char* target_saved = target;

        for(; viewport_x < fb_width; viewport_x += 2) {

            __m128d loaded_x = _mm_set1_pd(0);
            __m128d loaded_y = _mm_set1_pd(0);
            __m128d loaded_x_square = loaded_x;
            __m128d loaded_y_square = loaded_y;

            __m128d fractal_x_sse = _mm_set_pd(fractal_x, fractal_x + x_step);
            __m128d fractal_y_sse = _mm_set1_pd(fractal_y);

            int i = 0;
            __m128i amount = _mm_set1_epi32(0);

            while(i++ < 50) {
                __m128d mixed = _mm_mul_pd(loaded_x, loaded_y);

                loaded_x = _mm_sub_pd(loaded_x_square, loaded_y_square);
                loaded_y = _mm_mul_pd(mixed, two_sse);

                loaded_x = _mm_add_ps(loaded_x, fractal_x_sse);
                loaded_y = _mm_add_ps(loaded_y, fractal_y_sse);

                loaded_x_square = _mm_mul_pd(loaded_x, loaded_x);
                loaded_y_square = _mm_mul_pd(loaded_y, loaded_y);

                __m128d distances = _mm_add_pd(loaded_x_square, loaded_y_square);

                __m128d mask = _mm_cmple_pd(distances, square_radius_sse);
                if(!_mm_test_all_ones(mask)) break;

                mask = _mm_srl_epi64(mask, shift_mask);
                amount = _mm_add_epi64(mask, amount);
            }

            target[0] = amount[0] * 4;
            target[1] = amount[0] * 4;
            target[2] = amount[0] * 4;

            target[4] = amount[1] * 4;
            target[5] = amount[1] * 4;
            target[6] = amount[1] * 4;

            fractal_x += sse_x_step;
            target += 8;
        }

        target = target_saved + fb_width * 4;

        fractal_y += y_step;
    }

}

void write_png_file(char* file_name, int width, int height, png_byte bit_depth, png_byte color_type, char* buffer) {
    png_byte** row_pointers = NULL;
    FILE* fp = NULL;

    do {
        row_pointers = (png_byte**) malloc(sizeof(png_byte*) * height);
        if(!row_pointers) {
            printf("Allocation failed\n");
            break;
        }
        for(int i = 0; i < height; i++) {
            row_pointers[i] = (png_byte*) malloc(4 * width);
            if(!row_pointers[i]) {
                printf("Allocation failed\n");
                break;
            }
        }

        for(int y = 0, i = 0; y < height; y++) {
            for(int x = 0; x < width * 4; x += 4) {
                row_pointers[y][x] = buffer[i++];
                row_pointers[y][x + 1] = buffer[i++];
                row_pointers[y][x + 2] = buffer[i++];
                row_pointers[y][x + 3] = 255;
                i++;
            }
        }

        fp = fopen(file_name, "wb");
        if(!fp) {
            printf("Open file failed\n");
            break;
        }

        png_struct* png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL); //create structure for write
        if(!png) {
            printf("Create write struct failed\n");
            break;
        }

        png_infop info = png_create_info_struct(png);
        if(!info) {
            printf("Create info struct failed\n");
            break;
        }

        if(setjmp(png_jmpbuf(png))) {
            printf("setjmp failed\n");
        }

        png_init_io(png, fp); //initialize file output
        png_set_IHDR( //set image properties
                png, //pointer to png_struct
                info, //pointer to info_struct
                width, //image width
                height, //image height
                8, //color depth
                PNG_COLOR_TYPE_RGBA, //color type
                PNG_INTERLACE_NONE, //interlace type
                PNG_COMPRESSION_TYPE_DEFAULT, //compression type
                PNG_FILTER_TYPE_DEFAULT //filter type
        );
        png_write_info(png, info); //write png image information to file
        png_write_image(png, row_pointers); //the thing we gathered here for
        png_write_end(png, NULL);
        printf("Image was created successfully\nCheck %s file\n", file_name);
    } while(0);

    if(fp) {
        fclose(fp);
    }

    for(int i = 0; i < height; i++) {
        if(row_pointers[i]) {
            free(row_pointers[i]);
        }
    }

    if(row_pointers) {
        free(row_pointers);
    }
}

void draw_messed_thing() {

    int resolution_factor = 4;

    int width = 4096 * 4 / resolution_factor;
    int height = 2840 * 4 / resolution_factor;
    char* buffer = calloc(width * height * 4, sizeof(char));

    messed_thing_drawer(buffer, width, height, 0.0001 * resolution_factor, -4, -2.3);

    write_png_file("mess.png", width, height, 8, PNG_COLOR_TYPE_RGB_ALPHA, buffer);
}