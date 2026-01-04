#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

#include "MyExtern.h"

#include "./objects/GameObject.h"

class Ball;

GLuint vertexShader;
GLuint fragmentShader;
GLuint shaderProgramID;

GLuint shadowVertexShader;
GLuint shadowFragmentShader;
GLuint shadowShaderProgramID;

GLuint depthMapFBO;
GLuint depthMap;
const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

glm::mat4 lightSpaceMatrix;

// screen size
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

glm::vec3 cameraPos(0.0f, 400.0f, 0.0f);

glm::mat4 gSubViewMat = glm::lookAt(
    cameraPos,
    glm::vec3(0.0f, 106.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, -1.0f)
);

glm::mat4 gProjMat = glm::perspective(
    glm::radians(45.0f),
    (float)SCR_WIDTH / SCR_HEIGHT,
    0.1f,
    2000.0f                        // [수정] 100.0f -> 1000.0f (이제 700 거리의 물체도 보임)
);

unsigned int gLightsCount = 3;
Light gLights[5] = {
    { glm::vec3(0.0f, 400.0f, 200.0f), glm::vec3(0.9f, 0.9f, 0.85f) },
    { glm::vec3(300.0f, 200.0f, 300.0f), glm::vec3(0.3f, 0.3f, 0.4f) },
    { glm::vec3(-300.0f, 200.0f, -300.0f), glm::vec3(0.3f, 0.25f, 0.2f) },
    
    // inactive
    { glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f) },
    { glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f) }
};

std::vector<std::shared_ptr<GameObject>> world;
Collider* gCollider = nullptr;
CameraMain* cameraMain = nullptr;

std::vector<GameMode*> gModeStack;

int gCurrTime = 0;
float pixel_per_mm = 0.1f;
float pixel_per_cm = pixel_per_mm * 10.0f;

std::vector<CollisionPair> gPairs;

GameScore gScore;
CollisionInfo gCollisionInfo;

bool gHasReady = true;
float gShootPower = 0;