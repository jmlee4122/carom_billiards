#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/glm/glm.hpp>

class GameObject;
class GameMode;

extern GLuint vertexShader;
extern GLuint fragmentShader;
extern GLuint shaderProgramID;

extern glm::mat4 gViewMat;
extern glm::mat4 gProjMat;

// screen size
extern unsigned int SCR_WIDTH;
extern unsigned int SCR_HEIGHT;

// light
extern glm::vec3 gLightPos;
extern glm::vec3 gLightColor;

// game objects
extern std::vector<std::unique_ptr<GameObject>> world;

// game mode stack
extern std::vector<GameMode*> gModeStack;