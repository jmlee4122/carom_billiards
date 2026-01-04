#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/glm/glm.hpp>

#include "MyStruct.h"

class GameObject;
class GameMode;
class Collider;
class CameraMain;

struct GameScore;
struct CollisionInfo;

extern GLuint vertexShader;
extern GLuint fragmentShader;
extern GLuint shaderProgramID;

// shadow shaders
extern GLuint shadowVertexShader;
extern GLuint shadowFragmentShader;
extern GLuint shadowShaderProgramID;

// showdow map
extern GLuint depthMapFBO;
extern GLuint depthMap;
extern const unsigned int SHADOW_WIDTH, SHADOW_HEIGHT;

// [추가] 빛 공간 변환 행렬 (쉐이더로 넘겨줄 것)
extern glm::mat4 lightSpaceMatrix;

extern glm::vec3 cameraPos;
extern glm::mat4 gSubViewMat;
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
extern Light gLights[5];

// game objects
extern std::vector<std::shared_ptr<GameObject>> world;
extern Collider* gCollider;
// main camera
extern CameraMain* cameraMain;

// game mode stack
extern std::vector<GameMode*> gModeStack;

// time
extern int gCurrTime;

extern float pixel_per_mm;
extern float pixel_per_cm;

// collision pair
extern std::vector<CollisionPair> gPairs;

// game score
extern GameScore gScore;
extern CollisionInfo gCollisionInfo;

extern bool gHasReady;
extern float gShootPower;