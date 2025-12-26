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
struct Light {
    glm::vec3 position;
    glm::vec3 color;
};
extern unsigned int gLightsCount;
extern Light gLights[4];

// game objects
extern std::vector<std::unique_ptr<GameObject>> world;

// game mode stack
extern std::vector<GameMode*> gModeStack;