#include "MyExtern.h"

GLuint vertexShader;
GLuint fragmentShader;
GLuint shaderProgramID;

glm::mat4 gViewMat = glm::mat4(1.0);
glm::mat4 gProjMat = glm::mat4(1.0);

// screen size
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;