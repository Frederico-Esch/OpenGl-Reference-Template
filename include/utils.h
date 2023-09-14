#ifndef UTILS_H
#define UTILS_H

#include <GLFW/glfw3.h>
#include <stdio.h>

typedef struct {
    int Width;
    int Height;
    const char* Title;
    GLFWwindow* window;
} Window_struct;

typedef struct {
    int Major;
    int Minor;
} Version_struct;

typedef enum {
    BACKGROUND = 1,
    DIAMOND = 2,
    CIRCLE = 4,
    ALL = BACKGROUND | DIAMOND | CIRCLE
} ToBeRendered;

typedef struct {
    GLuint* vao;
    GLsizei vao_size;

    GLuint* vbo;
    GLsizei vbo_size;

    GLuint* ebo;
    GLsizei ebo_size;

    GLuint* program;
    GLsizei program_size;
} Buffers_struct;

typedef struct {
    int channels, width, height, unit;
    const char* name;
    GLuint program;
} TextureInfo_struct;

typedef struct {
    GLuint* texture;
    TextureInfo_struct* texture_info;
    GLsizei texture_size;
} Texture_struct;

typedef struct {
    GLfloat X;
    GLfloat Y;
} Point;

typedef struct {
    GLfloat R;
    GLfloat G;
    GLfloat B;
} Color;

typedef struct {
    Point point;
    Color color;
    Point texCoord;
}Vertex;

typedef struct {
    GLint time;
} Uniforms;

typedef enum {
    VA_ATT_POS = 0,
    VA_ATT_COL = 1,
    VA_ATT_TEX = 2,
} VA_ATT;

typedef struct {
    Window_struct*  window;
    Buffers_struct* buffers;
    Texture_struct* textures;
    Uniforms* uniforms;
    int revise_circle;
    int reload_shaders;
    ToBeRendered to_render;
} State;

int read_circle_definition();

#endif
