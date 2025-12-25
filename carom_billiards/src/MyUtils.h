#pragma once

#include <gl/glew.h>
#include <gl/freeglut.h>

struct Model;
struct Vertex;
struct Face;


void read_newline(char* str);
void read_obj_file(const char* filename, Model* model);
void print_model_info(const Model* model);
void InitBuffers(GLuint& VAO, GLuint& VBO_pos, GLuint& VBO_nol, GLuint& EBO,
    Vertex* vData, Vertex* vNormal, Face* fData, size_t vCount, size_t fCount, size_t nCount);

char* filetobuf(const char* file);
void make_vertexShaders(const char* filename);
void make_fragmentShaders(const char* filename);
GLuint make_shaderProgram();