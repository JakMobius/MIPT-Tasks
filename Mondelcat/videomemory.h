#define GL_SILENCE_DEPRECATION

#ifndef MANDELBROT_VIDEOMEMORY_H
#define MANDELBROT_VIDEOMEMORY_H

#include <stdio.h>
#include <stdbool.h>
#include "SDL.h"
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#include <time.h>
#include "bmp_reader.h"

typedef struct proc_vm {

    bool has_window;
    int window_width;
    int window_height;
    int scale;

    int real_window_width;
    int real_window_height;

    void* framebuffer;

    SDL_GLContext sdl_context;
    SDL_Window *sdl_window;
    GLenum gl_program;
    GLenum gl_texture;
    GLenum gl_vao;

    GLint gl_program_texture_uniform;
    GLint gl_program_zoom_uniform;
    GLint gl_program_offset_uniform;

    GLuint gl_vertex_buffer;
    float zoom;
    float camera_x;
    float camera_y;

    bool allow_highres;
    bool allow_resize;
} s_proc_vm;

s_proc_vm* proc_vm_create(void);
void proc_vm_destroy(s_proc_vm* vm);
void proc_vm_release(s_proc_vm* thou);
void proc_vm_redraw(s_proc_vm* thou);
bool proc_vm_compile_shaders(s_proc_vm* thou);
bool proc_vm_init_window(s_proc_vm* thou, int width, int height);
void proc_vm_deinit_window(s_proc_vm* thou);
bool proc_vm_create_texture(s_proc_vm* thou);
bool proc_vm_create_vertex_buffer(s_proc_vm* thou);
bool proc_vm_create_vao(s_proc_vm* thou);
bool proc_vm_set_size(s_proc_vm* thou, int width, int height);
void proc_vm_set_allow_resize(s_proc_vm* thou, bool allow);
bool proc_vm_update_virtual_size(s_proc_vm* thou, int width, int height);

#endif //MANDELBROT_VIDEOMEMORY_H
