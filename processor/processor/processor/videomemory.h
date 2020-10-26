
#ifndef videomemory_h
#define videomemory_h

#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#include "../shared/file_io.h"
#include <time.h>

typedef enum proc_vm_result {
    PROC_VM_RESULT_OK,
    PROC_VM_RESULT_ERROR,
    PROC_VM_RESULT_SHADER_COMPILATION_ERROR,
    PROC_VM_RESULT_PROGRAM_LINK_ERROR
} e_proc_vm_state;

typedef struct proc_vm {
    
    bool quit_event_emitted;
    
    bool has_window;
    int window_width;
    int window_height;
    int scale;
    e_proc_vm_state state;
    
    void* framebuffer;
    
    SDL_GLContext sdl_context;
    SDL_Window *sdl_window;
    GLenum gl_program;
    GLenum gl_texture;
    GLenum gl_vao;
    
    GLenum gl_program_attribute_position;
    GLenum gl_program_texture_uniform;
    
    GLuint gl_vertex_buffer;
} s_proc_vm;

s_proc_vm* proc_vm_create(void);
void proc_vm_destroy(s_proc_vm* vm);
void proc_vm_redraw(s_proc_vm* thou);
void proc_vm_compile_shaders(s_proc_vm* thou);
void proc_vm_init_window(s_proc_vm* thou, int width, int height, void* framebuffer);
void proc_vm_create_texture(s_proc_vm* thou);
void proc_vm_create_vertex_buffer(s_proc_vm* thou);
void proc_vm_create_vao(s_proc_vm* thou);

#endif /* videomemory_h */
