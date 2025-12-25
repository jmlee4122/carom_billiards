#pragma once

#include <iostream>
#include <vector>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/glm/glm.hpp>


extern GLuint vertexShader;
extern GLuint fragmentShader;
extern GLuint shaderProgramID;

extern glm::mat4 gViewMat;
extern glm::mat4 gProjMat;

// screen size
extern unsigned int SCR_WIDTH;
extern unsigned int SCR_HEIGHT;