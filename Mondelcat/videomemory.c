#include "videomemory.h"

const char* VM_VERTEX_SHADER = "#version 330 core\n"
                               "\n"
                               "uniform sampler2D u_texture;\n"
                               "uniform float u_zoom;\n"
                               "uniform vec2 u_offset;\n"
                               "layout(location = 0) in vec2 position;\n"
                               "out vec2 vertex_position;\n"
                               "\n"
                               "void main()\n"
                               "{\n"
                               "    vertex_position = position * u_zoom + u_offset;\n"
                               "    gl_Position = vec4(position, 0.0, 1.0);\n"
                               "}";
const char* VM_FRAGMENT_SHADER = "#version 330 core\n"
                                 "\n"
                                 "uniform sampler2D u_texture;\n"
                                 "in vec2 vertex_position;\n"
                                 "out vec4 color;\n"
                                 "\n"
                                 "void main(){\n"
                                 "    color = vec4(texture(u_texture, vertex_position).xyz, 1.0);\n"
                                 "}";

static bool sdl_initialized = false;

s_proc_vm* proc_vm_create() {

    s_proc_vm* new_vm = calloc(1, sizeof(s_proc_vm));

    new_vm->has_window = false;
    new_vm->sdl_window = NULL;
    new_vm->scale = 1;
    new_vm->zoom = 0.5;
    new_vm->camera_x = 0.5;
    new_vm->camera_y = 0.5;

    return new_vm;
}

void proc_vm_deinit_window(s_proc_vm* thou) {
    if(thou->framebuffer) {
        free(thou->framebuffer);
        thou->framebuffer = NULL;
        if(thou->has_window) {
            SDL_DestroyWindow(thou->sdl_window);
            thou->sdl_window = NULL;
            thou->has_window = false;
        }
    }

    if(thou->sdl_context) SDL_GL_DeleteContext(thou->sdl_context);
    if(thou->gl_vertex_buffer) glDeleteBuffers(1, &thou->gl_vertex_buffer);
    if(thou->gl_program) glDeleteProgram(thou->gl_program);
    if(thou->gl_texture) glDeleteTextures(1, &thou->gl_texture);
}

void proc_vm_destroy(s_proc_vm* thou) {
    proc_vm_deinit_window(thou);
}

void proc_vm_release(s_proc_vm* thou) {
    proc_vm_destroy(thou);
    free(thou);
}

void proc_vm_set_allow_resize(s_proc_vm* thou, bool allow) {
    thou->allow_resize = allow;

    SDL_SetWindowResizable(thou->sdl_window, allow);
}

bool init_sdl() {
    int error = SDL_Init(SDL_INIT_VIDEO);

    if(error == -1) return false;

    sdl_initialized = true;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    return 1;
}

void checkerr() {
    int err = glGetError();
    if(err != 0) {
        printf("ouch %d\n", err);
    }
}

bool proc_vm_init_window(s_proc_vm* thou, int width, int height) {

    if(!sdl_initialized) {
        init_sdl();
    }

    thou->has_window = true;
    thou->sdl_window = SDL_CreateWindow("Some Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width * thou->scale, height * thou->scale, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
    thou->window_width = width;
    thou->window_height = height;

    SDL_GL_GetDrawableSize(thou->sdl_window, &thou->real_window_width, &thou->real_window_height);

    if(thou->allow_highres) {
        thou->framebuffer = calloc(buffer_size(thou->real_window_width, thou->real_window_height), sizeof(char));
    } else {
        thou->framebuffer = calloc(buffer_size(thou->window_width, thou->window_height), sizeof(char));
    }

    if(!thou->framebuffer) {
        proc_vm_deinit_window(thou);
        return false;
    }

    if(thou->sdl_window == NULL) {
        proc_vm_deinit_window(thou);
        return false;
    }

    thou->sdl_context = SDL_GL_CreateContext(thou->sdl_window);

    if(!thou->sdl_context) {
        proc_vm_deinit_window(thou);
        return false;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0);

    SDL_GL_SetSwapInterval(1);

    if( !proc_vm_create_vertex_buffer(thou) ||
        !proc_vm_create_vao(thou) ||
        !proc_vm_create_texture(thou) ||
        !proc_vm_compile_shaders(thou)) {
        proc_vm_deinit_window(thou);
        return false;
    }

    return true;
}

GLuint proc_vm_shader(s_proc_vm* thou, const char* source, GLenum type) {
    GLuint shader = glCreateShader(type);

    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

    if (isCompiled == GL_FALSE) {
        GLint length;

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

        if (length > 0)
        {
            GLchar* log = malloc(length);

            glGetShaderInfoLog(shader, length, NULL, &log[0]);

            printf("Failed to compile shader: %s\n", log);

            free(log);
        } else {
            printf("Failed to compile shader\n");
        }

        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

bool proc_vm_create_vao(s_proc_vm* thou) {
    glGenVertexArrays(1, &(thou->gl_vao));
    glBindVertexArray(thou->gl_vao);
    if(!thou->gl_vao) return false;

    glBindBuffer(GL_ARRAY_BUFFER, thou->gl_vertex_buffer);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * sizeof(float), 0);

    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    return true;
}

bool proc_vm_compile_shaders(s_proc_vm* thou) {

    GLuint vertProg = proc_vm_shader(thou, VM_VERTEX_SHADER, GL_VERTEX_SHADER);
    GLuint fragProg = proc_vm_shader(thou, VM_FRAGMENT_SHADER, GL_FRAGMENT_SHADER);

    if(!vertProg || !fragProg) return false;

    thou->gl_program = glCreateProgram();

    if(!thou->gl_program) return false;

    glAttachShader(thou->gl_program, vertProg);
    glAttachShader(thou->gl_program, fragProg);
    glLinkProgram(thou->gl_program);

    glClearColor(0, 0, 0, 1);

    glValidateProgram(thou->gl_program);

    GLint isLinked = 0;
    glGetProgramiv(thou->gl_program, GL_LINK_STATUS, (int*)&isLinked);

    if (isLinked == GL_FALSE) {
        GLint length = 0;
        glGetProgramiv(thou->gl_program, GL_INFO_LOG_LENGTH, &length);

        if (length > 0) {
            char* description = malloc(length);
            glGetProgramInfoLog(thou->gl_program, length, &length, description);

            glDeleteProgram(thou->gl_program);

            printf("Failed to link program: %s\n", description);

            free(description);
        } else {
            printf("Failed to link program\n");
        }

        return false;
    }

    glDetachShader(thou->gl_program, fragProg);
    glDetachShader(thou->gl_program, vertProg);

    glUseProgram(thou->gl_program);

    thou->gl_program_texture_uniform = glGetUniformLocation(thou->gl_program, "u_texture");
    thou->gl_program_zoom_uniform    = glGetUniformLocation(thou->gl_program, "u_zoom");
    thou->gl_program_offset_uniform  = glGetUniformLocation(thou->gl_program, "u_offset");

    glUniform1i(thou->gl_program_texture_uniform, 0);

    return true;
}

bool proc_vm_create_texture(s_proc_vm* thou) {
    if(thou->gl_texture) {
        glDeleteTextures(1, &thou->gl_texture);
    }
    GLuint texture = 0;

    glGenTextures(1, &texture);
    if(!texture) return false;
    glActiveTexture(GL_TEXTURE0);

    if(thou->allow_highres) {
        unsigned char* buffer = malloc(thou->real_window_width * thou->real_window_height * 4);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, thou->real_window_width, thou->real_window_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
        free(buffer);
    } else {
        unsigned char* buffer = malloc(thou->window_width * thou->window_height * 4);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, thou->window_width, thou->window_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
        free(buffer);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    thou->gl_texture = texture;

    return true;
}

bool proc_vm_create_vertex_buffer(s_proc_vm* thou) {
    glGenBuffers(1, &thou->gl_vertex_buffer);

    if(thou->gl_vertex_buffer == 0) return false;
    glBindBuffer(GL_ARRAY_BUFFER, thou->gl_vertex_buffer);

    float data[] = {
            -1, 1,
            1, 1,
            1, -1,
            -1, 1,
            -1, -1,
            1, -1
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

    return true;
}

void proc_vm_redraw(s_proc_vm* thou) {

    glViewport(0, 0, thou->real_window_width, thou->real_window_height);

    if(thou->allow_highres) {
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, thou->real_window_width / thou->scale, thou->real_window_height / thou->scale, GL_RGBA, GL_UNSIGNED_BYTE, thou->framebuffer);
    } else {
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, thou->window_width, thou->window_height, GL_RGBA, GL_UNSIGNED_BYTE, thou->framebuffer);
    }

    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(thou->gl_program);

    glUniform1f(thou->gl_program_zoom_uniform, thou->zoom);
    glUniform2f(thou->gl_program_offset_uniform, thou->camera_x, thou->camera_y);

    glBindVertexArray(thou->gl_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    SDL_GL_SwapWindow(thou->sdl_window);
}

bool proc_vm_update_virtual_size(s_proc_vm* thou, int width, int height) {
    SDL_GL_GetDrawableSize(thou->sdl_window, &thou->real_window_width, &thou->real_window_height);

    char* new_buffer = NULL;

    if(thou->allow_highres) {
        new_buffer = realloc(thou->framebuffer, buffer_size(thou->real_window_width, thou->real_window_height));
    } else {
        new_buffer = realloc(thou->framebuffer, buffer_size(width, height));
    }

    if(!new_buffer) {
        return false;
    }

    thou->framebuffer = new_buffer;

    thou->window_width = width;
    thou->window_height = height;

    proc_vm_create_texture(thou);

    return true;
}

void show_flags(int flags) {

    printf("\nFLAGS ENABLED: ( %d )\n", flags);
    printf("=======================\n");
    if(flags & SDL_WINDOW_FULLSCREEN)         printf("SDL_WINDOW_FULLSCREEN\n");
    if(flags & SDL_WINDOW_OPENGL)             printf("SDL_WINDOW_OPENGL\n");
    if(flags & SDL_WINDOW_SHOWN)              printf("SDL_WINDOW_SHOWN\n");
    if(flags & SDL_WINDOW_HIDDEN)             printf("SDL_WINDOW_HIDDEN\n");
    if(flags & SDL_WINDOW_BORDERLESS)         printf("SDL_WINDOW_BORDERLESS\n");
    if(flags & SDL_WINDOW_RESIZABLE)          printf("SDL_WINDOW_RESIZABLE\n");
    if(flags & SDL_WINDOW_MINIMIZED)          printf("SDL_WINDOW_MINIMIZED\n");
    if(flags & SDL_WINDOW_MAXIMIZED)          printf("SDL_WINDOW_MAXIMIZED\n");
    if(flags & SDL_WINDOW_INPUT_GRABBED)      printf("SDL_WINDOW_INPUT_GRABBED\n");
    if(flags & SDL_WINDOW_INPUT_FOCUS)        printf("SDL_WINDOW_INPUT_FOCUS\n");
    if(flags & SDL_WINDOW_MOUSE_FOCUS)        printf("SDL_WINDOW_MOUSE_FOCUS\n");
    if(flags & SDL_WINDOW_FULLSCREEN_DESKTOP) printf("SDL_WINDOW_FULLSCREEN_DESKTOP\n");
    if(flags & SDL_WINDOW_FOREIGN)            printf("SDL_WINDOW_FOREIGN\n");
}

bool proc_vm_set_size(s_proc_vm* thou, int width, int height) {
    if(!thou->has_window) return false;

    Uint32 flags = SDL_GetWindowFlags(thou->sdl_window);

    show_flags(SDL_GetWindowFlags(thou->sdl_window));

    if((flags & SDL_WINDOW_FULLSCREEN) || (flags & SDL_WINDOW_FULLSCREEN_DESKTOP)) {
        SDL_SetWindowFullscreen(thou->sdl_window, 0);
    }

    if(flags & SDL_WINDOW_MAXIMIZED) {
        printf("Minimizing window\n");
        SDL_MinimizeWindow(thou->sdl_window);
        SDL_RestoreWindow(thou->sdl_window);
    }

    SDL_SetWindowSize(thou->sdl_window, width * thou->scale, height * thou->scale);

    if(!proc_vm_update_virtual_size(thou, width, height)) {
        SDL_SetWindowSize(thou->sdl_window, thou->window_width * thou->scale, thou->window_height * thou->scale);
        SDL_GL_GetDrawableSize(thou->sdl_window, &thou->real_window_width, &thou->real_window_height);
        return false;
    }
    return true;
}