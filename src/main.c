#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <utils.h>

#define DEBUG_INFO
#define VERTEX_SHADER_PATH "./shaders/vertex.shader"
#define FRAGMENT_SHADER_PATH "./shaders/fragment.shader"
#define RETRY_COMPILATION_SHADERS 3

static State main_state;

void exit_error(const char* error_msg);

void error_callback(int error, const char* description) {
    printf_s("ERROR %d", error);
    puts(": ");
    exit_error(description);
}

void exit_error(const char* error_msg) {
        printf_s("%s\n", error_msg);
        if (main_state.window->window != NULL) {
            glfwSetWindowShouldClose(main_state.window->window, 1);
            return;
        }
        exit(1);
}

void setup_glfw_version(int major, int minor){
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

#ifdef DEBUG_INFO
    printf_s("OPEN GL VERSION: %s\n",  glfwGetVersionString());
#endif
}

void load_glad() {
    int major_minor = gladLoadGL(glfwGetProcAddress);
    if (major_minor == 0) exit_error("COULDN'T LOAD GL CORRECTLY\n");
#ifdef DEBUG_INFO
    int major = major_minor/10000;
    int minor = major_minor - major_minor;
    printf("GLAD version: %d.%d\n", major, minor);
#endif
}

static void key_callback(GLFWwindow* window , int key, int scan_code, int action, int mods) {
    (void) scan_code;
    if (action == GLFW_RELEASE) return;

    switch (key) {
        case GLFW_KEY_Q:
            if (mods == 0)
                glfwSetWindowShouldClose(window, 1);
        break;
        case GLFW_KEY_ESCAPE:
            if (mods == 0)
                glfwSetWindowShouldClose(window, 1);
        break;

        case GLFW_KEY_R:
            if (mods == 0) {
                main_state.revise_circle = 1;
            }
            else {
                main_state.reload_shaders = 1;
            }
        break;

        case GLFW_KEY_RIGHT:
            switch(main_state.to_render){
                case BACKGROUND:
                    main_state.to_render = DIAMOND;
                break;
                case DIAMOND:
                    main_state.to_render = CIRCLE;
                break;
                case CIRCLE:
                    main_state.to_render = ALL;
                break;
                case ALL:
                    main_state.to_render = BACKGROUND;
                break;
            }
            break;
        case GLFW_KEY_LEFT:
            switch(main_state.to_render){
                case BACKGROUND:
                    main_state.to_render = ALL;
                break;
                case DIAMOND:
                    main_state.to_render = BACKGROUND;
                break;
                case CIRCLE:
                    main_state.to_render = DIAMOND;
                break;
                case ALL:
                    main_state.to_render = CIRCLE;
                break;
            }
            break;

        case GLFW_KEY_UP:
            if (main_state.uniforms != NULL) {
                //float time = 0;
                //glGetUniformfv(*main_state.buffers->program, main_state.uniforms->time, &time);
                //glUniform1f(main_state.uniforms->time, time+.1f);
            }
        break;
        case GLFW_KEY_DOWN:
            if (main_state.uniforms != NULL) {
                //float time = 0;
                //glGetUniformfv(*main_state.buffers->program, main_state.uniforms->time, &time);
                //glUniform1f(main_state.uniforms->time, time-.1f);
            }
        break;
    }

}

void window_resize_callback(GLFWwindow* window, int width, int height) {
    (void) window;
    (void) width;
    (void) height;

    main_state.window->Width = width;
    main_state.window->Height = height;

    glViewport(0, 0, width, height);
}

void setup_window_and_callbacks(Window_struct* window, GLFWkeyfun _key_callback, GLFWwindowsizefun _resize_callback) {
    window->window = glfwCreateWindow( window->Width, window->Height, window->Title, NULL, NULL);
    if (!window->window) exit_error("Couldn't create a window");
    glfwMakeContextCurrent(window->window);

    glfwSetKeyCallback(window->window, _key_callback);
    //glfwSetFramebufferSizeCallback(window->window, frame_buffer_resize_callback);
    glfwSetWindowSizeCallback(window->window, _resize_callback);

#ifdef DEBUG_INFO
    int result = glfwGetWindowAttrib(window->window, GLFW_OPENGL_PROFILE);
    printf_s("PROFILE: %s | ", result == GLFW_OPENGL_CORE_PROFILE == result ? "CORE" : "NOT CORE");
    printf_s("VERSION: %d.%d\n", glfwGetWindowAttrib(window->window, GLFW_CONTEXT_VERSION_MAJOR), glfwGetWindowAttrib(window->window, GLFW_CONTEXT_VERSION_MINOR));
#endif
}

void generate_buffers(Buffers_struct* buffers) {
    if (buffers->vao_size > 0){
        buffers->vao = malloc(buffers->vao_size*sizeof(GLuint));
        if (buffers->vao == NULL) exit_error("Error Creating VAO");
        glCreateVertexArrays(buffers->vao_size, buffers->vao);

#ifdef DEBUG_INFO
        printf_s("%d VAO CREATED ", (int)buffers->vao_size);
#endif
    }

    if (buffers->vbo_size > 0) {
        buffers->vbo = malloc(buffers->vbo_size*sizeof(GLuint));
        if (buffers->vbo == NULL) exit_error("Error Creating VBO");
        glCreateBuffers(buffers->vbo_size, buffers->vbo); //GL_ARRAY_BUFFER

#ifdef DEBUG_INFO
        printf_s("%d VBO CREATED ", (int)buffers->vbo_size);
#endif
    }

    if (buffers->ebo_size > 0) {
        buffers->ebo = malloc(buffers->ebo_size*sizeof(GLuint));
        if (buffers->ebo == NULL) exit_error("Error Creating EBO");
        glCreateBuffers(buffers->ebo_size, buffers->ebo); //GL_ELEMENT_ARRAY_BUFFER

#ifdef DEBUG_INFO
        printf_s("%d EBO CREATED", (int)buffers->ebo_size);
#endif
    }

#ifdef DEBUG_INFO
    printf_s("\n");
#endif
}

char* read_shader(char* path) {
    FILE* shader;
    char* content;
    size_t size;

    int error = fopen_s(&shader, path, "r");
    if (error) {
        char text[1000];
        sprintf_s((char*)text, 1000, "ERROR reading %s shader", path);
        exit_error(text);
        return "";
    }

    fseek(shader, 0, SEEK_END);
    size = ftell(shader)+1;
    content = malloc(size);
    if (content == NULL) {
        char text[1000];
        sprintf_s((char*)text, 1000, "ERROR allocing buffer for %s shader", path);
        exit_error(text);
        return "";
    }
    fseek(shader, 0, SEEK_SET);
    size_t end = fread(content, 1, size, shader);
    content[end] = '\0';

    fclose(shader);

    return content;
}

void shader_log(GLuint shader, const char* shader_type) {
    GLint logSize;
    GLint actualSize;
    GLchar* log;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
    if (logSize > 0)
    {
        log = malloc(logSize*sizeof(GLchar));
        glGetShaderInfoLog(shader, logSize, &actualSize, log);
        printf_s("%s SHADER INFO: %s\n", shader_type, log);
        free(log);
    }
}

void load_shaders(GLuint* prog) {
    GLint success;
    GLuint vertex_shader = 0;
    GLuint fragment_shader = 0;
    *prog = glCreateProgram();
    GLboolean hasCompiled = GL_FALSE;
    int tried = 0;

    while (!hasCompiled && tried < RETRY_COMPILATION_SHADERS) {
        const char* vertex_shader_text = read_shader(VERTEX_SHADER_PATH);
        vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
        glCompileShader(vertex_shader);
        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
#ifdef DEBUG_INFO
        shader_log(vertex_shader, "Vertex");
#endif
        if (success == GL_FALSE)
            printf_s("ERROR COMPILING VERTEX SHADER TRIED %d\n", ++tried);
        else
            hasCompiled = GL_TRUE;
        free((void*)vertex_shader_text);
    }
    if (!hasCompiled) {
            exit_error("ERROR COMPILING VERTEX SHADER");
            goto vertex_end;
    }


    hasCompiled = GL_FALSE; tried = 0;
    while (!hasCompiled && tried < RETRY_COMPILATION_SHADERS) {
        const char* fragment_shader_text = read_shader(FRAGMENT_SHADER_PATH);
        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
        glCompileShader(fragment_shader);
#ifdef DEBUG_INFO
        shader_log(fragment_shader, "Fragment");
#endif
        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
        if (success == GL_FALSE)
            printf_s("ERROR COMPILING FRAG SHADER, TRIED %d\n", ++tried);
        else
            hasCompiled = GL_TRUE;
        free((void*)fragment_shader_text);
    }
    if (!hasCompiled) {
        exit_error("ERROR COMPILING FRAG SHADER");
        goto frag_end;
    }

    glAttachShader(*prog, vertex_shader);
    glAttachShader(*prog, fragment_shader);
    glLinkProgram(*prog);

    glGetProgramiv(*prog, GL_LINK_STATUS, &success);

    if (success == GL_FALSE) {
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        glDeleteProgram(*prog);
        exit_error("ERROR LINKING PROGRAM");
    }

    glUseProgram(*prog);

frag_end:
    glDetachShader(*prog, fragment_shader);
    glDeleteShader(fragment_shader);
vertex_end:
    glDetachShader(*prog, vertex_shader);
    glDeleteShader(vertex_shader);
}

void delete_buffers(Buffers_struct* buffers) {
    if (buffers->vbo_size > 0)
        glDeleteBuffers(buffers->vbo_size, buffers->vbo);
    if (buffers->ebo_size > 0)
        glDeleteBuffers(buffers->ebo_size, buffers->ebo);
    if (buffers->vao_size > 0)
        glDeleteVertexArrays(buffers->vao_size, buffers->vao);
}

void unbound_buffers() {
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void draw_routine(){
    //glBindVertexArray(buffer->vao[0]);

    //glBindBuffer(GL_ARRAY_BUFFER, buffer->vbo[0]);
    Vertex data[] = {
        (Vertex) { .point = (Point){ .X = -1, .Y = -1 }, .color = (Color) { .R = 1, .G = 0, .B = 0 } },
        (Vertex) { .point = (Point){ .X =  1, .Y = -1 }, .color = (Color) { .R = 0, .G = 1, .B = 0 } },
        (Vertex) { .point = (Point){ .X = .0, .Y =  1 }, .color = (Color) { .R = 0, .G = 0, .B = 1 } }
    };
    glBufferData(GL_ARRAY_BUFFER, 3*sizeof(Vertex), data, GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->ebo[0]);
    GLuint indexes[] = { 0, 1, 2 };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3*sizeof(GLuint), indexes, GL_STATIC_DRAW);

    //glBindVertexArray(0);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    unbound_buffers();
}

void background_flag(GLuint vbo, GLuint ebo) {
    Vertex data[] = {
        (Vertex) { .point = (Point) { .X = -1, .Y = -1 }, .color = (Color) { .R = 0, .G = 156.f/255.f, .B = 59.f/255.f } },
        (Vertex) { .point = (Point) { .X = -1, .Y =  1 }, .color = (Color) { .R = 0, .G = 156.f/255.f, .B = 59.f/255.f } },
        (Vertex) { .point = (Point) { .X =  1, .Y = -1 }, .color = (Color) { .R = 0, .G = 156.f/255.f, .B = 59.f/255.f } },
        (Vertex) { .point = (Point) { .X =  1, .Y =  1 }, .color = (Color) { .R = 0, .G = 156.f/255.f, .B = 59.f/255.f } },
    };
    glNamedBufferStorage(vbo, 4*sizeof(Vertex), data, GL_DYNAMIC_STORAGE_BIT);
    //glBufferData(GL_ARRAY_BUFFER, 4*sizeof(Vertex), data, GL_STATIC_READ);

    GLuint indexes[] = { 0, 1, 2, 1, 2, 3 };
    glNamedBufferStorage(ebo, 6*sizeof(GLuint), indexes, GL_DYNAMIC_STORAGE_BIT);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(GLuint), indexes, GL_STATIC_READ);
    //unbound_buffers();
}

void diamond_flag(GLuint vbo, GLuint ebo) {
    Vertex data[] = {
        (Vertex) { .point = (Point) { .X = -1, .Y =  0 }, .color = (Color) { .R = 255.f/255.f, .G = 223.f/255.f, .B = 0.f/255.f } },
        (Vertex) { .point = (Point) { .X =  0, .Y =  1 }, .color = (Color) { .R = 255.f/255.f, .G = 223.f/255.f, .B = 0.f/255.f } },
        (Vertex) { .point = (Point) { .X =  0, .Y = -1 }, .color = (Color) { .R = 255.f/255.f, .G = 223.f/255.f, .B = 0.f/255.f } },
        (Vertex) { .point = (Point) { .X =  1, .Y =  0 }, .color = (Color) { .R = 255.f/255.f, .G = 223.f/255.f, .B = 0.f/255.f } },
    };
    glNamedBufferStorage(vbo, 4*sizeof(Vertex), data, GL_DYNAMIC_STORAGE_BIT);
    //glBufferData(GL_ARRAY_BUFFER, 4*sizeof(Vertex), data, GL_STATIC_READ);

    GLuint indexes[] = { 0, 1, 2, 1, 2, 3 };
    glNamedBufferStorage(ebo, 6*sizeof(GLuint), indexes, GL_DYNAMIC_STORAGE_BIT);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(GLuint), indexes, GL_STATIC_READ);
    //unbound_buffers();
}

void circle_flag(GLuint vbo, GLuint ebo, int vertexes) {
    const int amount_indexes = vertexes+1;
    const double radius = .3;
    const double step = (2.*M_PI)/((double)(vertexes - 1));

    Vertex *data = malloc(vertexes*sizeof(Vertex));
    GLuint* indexes = malloc(amount_indexes*sizeof(GLuint));

    for (int i = 1; i < vertexes; i ++) {
        indexes[i] = i;
        double angle = step*(double)(i-1);
        data[i] = (Vertex) {
            .point = (Point) { .X = (GLfloat)(radius*cos(angle)), .Y = (GLfloat)(radius*sin(angle)) },
            .color = (Color) { .R = 0, .G = 39.f/255.f, .B = 118.f/255.f }
        };
    }
    indexes[0] = 0;
    indexes[vertexes] = 1;
    data[0] = (Vertex) { .point = (Point) { .X = 0, .Y = 0 }, .color = (Color) { .R = 0, .G = 39.f/255.f, .B = 118.f/255.f } };

    
    glNamedBufferStorage(vbo, vertexes*sizeof(Vertex), data, GL_DYNAMIC_STORAGE_BIT);
    //glBufferData(GL_ARRAY_BUFFER, vertexes*sizeof(Vertex), data, GL_STREAM_DRAW);

    glNamedBufferStorage(ebo, amount_indexes*sizeof(GLuint), indexes, GL_DYNAMIC_STORAGE_BIT);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, amount_indexes*sizeof(GLuint), indexes ,GL_STREAM_DRAW);

    free(indexes);
    free(data);
}

void bind_buffer(Buffers_struct* buffers, size_t vao_index, size_t vbo_index, size_t ebo_index) {
    glBindVertexArray(buffers->vao[vao_index]);
    glBindBuffer(GL_ARRAY_BUFFER, buffers->vbo[vbo_index]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers->ebo[ebo_index]);
}

void load_uniforms(GLuint program, Uniforms* uniforms) {
    uniforms->time = glGetUniformLocation(program, "time");
}

void update_time(double* prev_time, double* time, Uniforms* uniforms) {
    double cur_time = glfwGetTime();
    double delta_time = cur_time - *prev_time;
    *time += delta_time;

    glUniform1f(uniforms->time, (float)delta_time);
}

void initialize(Window_struct* window, GLFWerrorfun _error_callback, GLFWkeyfun _key_callback, GLFWwindowsizefun _resize_callback) {
    if (!glfwInit()) exit_error("Error loading glfw");
    glfwSetErrorCallback(_error_callback);
    setup_glfw_version(4, 5);

    setup_window_and_callbacks(window, _key_callback, _resize_callback);

    load_glad();
}

void cleanup(Window_struct* window, Buffers_struct* buffers) {
    delete_buffers(buffers);
    for (int i = 0; i < buffers->program_size; i++)
        glDeleteProgram(buffers->program[i]);
    glfwDestroyWindow(window->window);
}

void setup_attribs(Buffers_struct* buffers){
    for(int i = 0; i < buffers->vao_size; i++){
        glEnableVertexArrayAttrib(buffers->vao[i], VA_ATT_POS);
        glEnableVertexArrayAttrib(buffers->vao[i], VA_ATT_COL);
        //glBindVertexArray(buffers->vao[i]);
        //glEnableVertexAttribArray(VA_ATT_POS);
        //glEnableVertexAttribArray(VA_ATT_COL);
        
        glVertexArrayAttribFormat(buffers->vao[i], VA_ATT_POS, 2, GL_FLOAT, GL_FALSE, 0);//No stride?? Strange
        glVertexArrayAttribFormat(buffers->vao[i], VA_ATT_COL, 3, GL_FLOAT, GL_FALSE, sizeof(Point));//No stride?? Strange
        //glVertexAttribPointer(VA_ATT_POS, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
        //glVertexAttribPointer(VA_ATT_COL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Point));
        
        glVertexArrayAttribBinding(buffers->vao[i], VA_ATT_POS, 0);//binding points dont make sense
        glVertexArrayAttribBinding(buffers->vao[i], VA_ATT_COL, 0);//binding points dont make sense
    }
}

void binding_buffers(Buffers_struct* buffers){
    for(int i = 0; i < buffers->vao_size; i++){
        glVertexArrayVertexBuffer(buffers->vao[i], 0, buffers->vbo[i], 0, sizeof(Vertex));
        glVertexArrayElementBuffer(buffers->vao[i], buffers->ebo[i]);
    }
}

int main() {
    Window_struct window; main_state.window = &window;
    Buffers_struct buffers; main_state.buffers = &buffers;
    Uniforms uniforms; main_state.uniforms = &uniforms;
    main_state.revise_circle  = 0;
    main_state.reload_shaders = 0;
    main_state.to_render = BACKGROUND;

    window = (Window_struct){
            .Width = 600,
            .Height = 400,
            .Title = "Principal"
    };

    initialize(
        &window,
        error_callback,
        key_callback,
        window_resize_callback
    );

    buffers.vao_size = 3; buffers.vbo_size = 3; buffers.ebo_size = 3;
    generate_buffers(&buffers);
    binding_buffers(&buffers);
    setup_attribs(&buffers);

    int circle_vertexes = read_circle_definition();
    for (int i = 0; i < buffers.vao_size; i++) {
        //bind_buffer(&buffers, i, i, i);
        switch (i) {
            case 0:
            background_flag(buffers.vbo[0], buffers.ebo[0]);
            break;
            case 1:
            diamond_flag(buffers.vbo[1], buffers.ebo[1]);
            //break;
            case 2:
            circle_flag(buffers.vbo[2], buffers.ebo[2], circle_vertexes);
            break;
        }
    }

    buffers.program_size = 1; buffers.program = malloc(buffers.program_size*sizeof(GLuint));
    load_shaders(buffers.program);

    load_uniforms(buffers.program[0], &uniforms);

    //draw_routine();
    double prev_time = 0;
    double time;
    while (!glfwWindowShouldClose(window.window)) {
        update_time(&prev_time, &time, &uniforms);
        if (main_state.revise_circle) {
            circle_vertexes = read_circle_definition();
            circle_flag(buffers.vbo[2], buffers.ebo[2], circle_vertexes);

            main_state.revise_circle = 0;
        }
        if (main_state.reload_shaders) {
            for (int i = 0; i < buffers.program_size; i++) glDeleteProgram(buffers.program[i]);
            load_shaders(buffers.program);
            load_uniforms(buffers.program[0], &uniforms);
            main_state.reload_shaders = 0;
        }
        glClear(GL_COLOR_BUFFER_BIT);
        ToBeRendered toBeRendered = main_state.to_render;

        glUseProgram(buffers.program[0]);
        if ((toBeRendered & BACKGROUND) == BACKGROUND) {
            glBindVertexArray(buffers.vao[0]);

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

            glBindVertexArray(0);
        }

        if ((toBeRendered & DIAMOND) == DIAMOND) {
            glBindVertexArray(buffers.vao[1]);

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

            glBindVertexArray(0);
        }

        if ((toBeRendered & CIRCLE) == CIRCLE) {
            glBindVertexArray(buffers.vao[2]);

            glDrawElements(GL_TRIANGLE_FAN, circle_vertexes+1, GL_UNSIGNED_INT, NULL);

            glBindVertexArray(0);
        }

        glfwSwapBuffers(window.window);
        glfwPollEvents();
    }

    cleanup(&window, &buffers);
    return 0;
}
