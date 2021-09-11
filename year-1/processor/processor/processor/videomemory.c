
#define GL_SILENCE_DEPRECATION
#include "videomemory.h"

const char* VM_VERTEX_SHADER = "#version 330 core\n"
                               "\n"
                               "uniform sampler2D u_texture;\n"
                               "layout(location = 0) in vec2 position;\n"
                               "out vec2 vertex_position;\n"
                               "\n"
                               "void main()\n"
                               "{\n"
                               "    vertex_position = position * 0.5 + vec2(0.5, 0.5);\n"
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

bool sdl_initialized = false;

s_proc_vm* proc_vm_create() {
    
    s_proc_vm* new_vm = calloc(1, sizeof(s_proc_vm));
    
    new_vm -> has_window = false;
    new_vm -> sdl_window = NULL;
    new_vm -> scale = 5;
    
    return new_vm;
}

void proc_vm_destroy(s_proc_vm* thou) {
    free(thou);
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

void proc_vm_init_window(s_proc_vm* thou, int width, int height, void* framebuffer) {
    
    if(!sdl_initialized) {
        init_sdl();
    }
    
    thou -> framebuffer = framebuffer;
    thou -> window_width = width;
    thou -> window_height = height;
    thou -> has_window = true;
    thou -> sdl_window = SDL_CreateWindow("Eder Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width * thou -> scale, height * thou -> scale, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    
    if(thou -> sdl_window == NULL) {
        thou -> has_window = NULL;
        thou -> state = PROC_VM_RESULT_ERROR;
        return;
    }
    
    thou -> sdl_context = SDL_GL_CreateContext(thou -> sdl_window);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0);
    
    SDL_GL_SetSwapInterval(1);
    
    proc_vm_create_vertex_buffer(thou);
    proc_vm_create_vao(thou);
    proc_vm_create_texture(thou);
    proc_vm_compile_shaders(thou);
}

GLuint proc_vm_shader(s_proc_vm* thou, const char* source, GLenum type) {
    if(thou -> state != PROC_VM_RESULT_OK) return 0;
    
    GLuint shader = glCreateShader(type);
    
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    
    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    
    if (isCompiled == GL_FALSE) {
        thou -> state = PROC_VM_RESULT_SHADER_COMPILATION_ERROR;
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

void proc_vm_create_vao(s_proc_vm* thou) {
    glGenVertexArrays(1, &(thou -> gl_vao));
    glBindVertexArray(thou -> gl_vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, thou -> gl_vertex_buffer);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * sizeof(float), 0);
    
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void proc_vm_compile_shaders(s_proc_vm* thou) {
    
    GLuint vertProg = proc_vm_shader(thou, VM_VERTEX_SHADER, GL_VERTEX_SHADER);
    GLuint fragProg = proc_vm_shader(thou, VM_FRAGMENT_SHADER, GL_FRAGMENT_SHADER);

    if(thou -> state != PROC_VM_RESULT_OK) return;

    thou -> gl_program = glCreateProgram();
    glAttachShader(thou -> gl_program, vertProg);
    glAttachShader(thou -> gl_program, fragProg);
    glLinkProgram(thou -> gl_program);
    
    glClearColor(0, 0, 0, 1);
    
    glValidateProgram(thou -> gl_program);
    
    GLint isLinked = 0;
    glGetProgramiv(thou -> gl_program, GL_LINK_STATUS, (int*)&isLinked);
    
    if (isLinked == GL_FALSE)
    {
        thou -> state = PROC_VM_RESULT_PROGRAM_LINK_ERROR;
        
        GLint length = 0;
        glGetProgramiv(thou -> gl_program, GL_INFO_LOG_LENGTH, &length);

        if (length > 0) {
            char* description = malloc(length);
            glGetProgramInfoLog(thou -> gl_program, length, &length, description);

            glDeleteProgram(thou -> gl_program);

            printf("Failed to link program: %s\n", description);

            free(description);
        } else {
            printf("Failed to link program\n");
        }
    }
    
    glDetachShader(thou -> gl_program, fragProg);
    glDetachShader(thou -> gl_program, vertProg);
    
    glUseProgram(thou -> gl_program);
    
    if(thou -> state != PROC_VM_RESULT_OK) return;
    
    thou -> gl_program_texture_uniform = glGetUniformLocation(thou -> gl_program, "u_texture");
    
    glUniform1i(thou -> gl_program_texture_uniform, 0);
}

void proc_vm_create_texture(s_proc_vm* thou) {
    GLuint texture = 0;
    
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    
    unsigned char* buffer = malloc(thou -> window_width * thou -> window_height * 4);
    
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, thou -> window_width, thou -> window_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    
    free(buffer);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    thou -> gl_texture = texture;
}

void proc_vm_create_vertex_buffer(s_proc_vm* thou) {
    glGenBuffers(1, &thou -> gl_vertex_buffer);
    
    glBindBuffer(GL_ARRAY_BUFFER, thou -> gl_vertex_buffer);
    
    float data[] = {
        -1, 1,
         1, 1,
         1, -1,
        -1, 1,
        -1, -1,
         1, -1
    };
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
}

//long long instructions_global;

void proc_vm_redraw(s_proc_vm* thou) {
    
    //printf("%lld\n", instructions_global);
    //instructions_global = 0;

    SDL_Event event = {0};
    SDL_PollEvent(&event);
    
    glViewport(0, 0, thou -> window_width * thou -> scale, thou -> window_height * thou -> scale);
    
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, thou -> window_width, thou -> window_height, GL_RGBA, GL_UNSIGNED_BYTE, thou -> framebuffer);
    
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(thou -> gl_program);
    
//    if(event.type == SDL_QUIT) {
//        thou -> quit_event_emitted = true;
//    }
    
    glBindVertexArray(thou -> gl_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    SDL_GL_SwapWindow(thou -> sdl_window);
}
