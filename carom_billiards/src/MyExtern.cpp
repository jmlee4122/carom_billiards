#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

#include "MyExtern.h"

#include "./objects/GameObject.h"

GLuint vertexShader;
GLuint fragmentShader;
GLuint shaderProgramID;

// screen size
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

glm::mat4 gViewMat = glm::lookAt(
    glm::vec3(300.0f, 300.0f, 300.0f), // 카메라 위치 (X, Y, Z) - Y를 올려서 위에서 내려다보게 함
    glm::vec3(0.0f, 0.0f, 0.0f),       // 바라보는 지점 (원점)
    glm::vec3(0.0f, 1.0f, 0.0f)        // 업 벡터
);

// 2. 투영 행렬 (Projection Matrix): Far Plane을 1000.0f 이상으로 늘려주세요.
glm::mat4 gProjMat = glm::perspective(
    glm::radians(45.0f),
    (float)SCR_WIDTH / SCR_HEIGHT,
    0.1f,
    1000.0f                        // [수정] 100.0f -> 1000.0f (이제 700 거리의 물체도 보임)
);

// light
unsigned int gLightsCount = 4;
Light gLights[4] = {
    { glm::vec3(500.0f, 500.0f, 500.0f), glm::vec3(1.0f, 1.0f, 1.0f) },
    { glm::vec3(500.0f, 500.0f, -500.0f), glm::vec3(1.0f, 1.0f, 1.0f) },
    { glm::vec3(-500.0f, 500.0f, 500.0f), glm::vec3(1.0f, 1.0f, 1.0f) },
    { glm::vec3(-500.0f, 500.0f, -500.0f), glm::vec3(1.0f, 1.0f, 1.0f) }
};

std::vector<std::unique_ptr<GameObject>> world;
std::vector<GameMode*> gModeStack;