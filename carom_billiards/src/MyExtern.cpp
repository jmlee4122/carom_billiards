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

glm::vec3 cameraPos(300.0f, 300.0f, 300.0f);

glm::mat4 gViewMat = glm::lookAt(
    cameraPos,
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f)
);

// 2. 투영 행렬 (Projection Matrix): Far Plane을 1000.0f 이상으로 늘려주세요.
glm::mat4 gProjMat = glm::perspective(
    glm::radians(45.0f),
    (float)SCR_WIDTH / SCR_HEIGHT,
    0.1f,
    2000.0f                        // [수정] 100.0f -> 1000.0f (이제 700 거리의 물체도 보임)
);

// 조명 개수를 줄이고 색상/강도 조정
unsigned int gLightsCount = 3;
Light gLights[5] = {
    // Main Light (그림자 투사) - 약간 부드러운 흰색
    { glm::vec3(0.0f, 400.0f, 200.0f), glm::vec3(0.9f, 0.9f, 0.85f) },
    
    // Fill Light (보조 조명) - 약한 파란 톤
    { glm::vec3(300.0f, 200.0f, 300.0f), glm::vec3(0.3f, 0.3f, 0.4f) },
    
    // Rim Light (외곽 조명) - 약한 따뜻한 톤
    { glm::vec3(-300.0f, 200.0f, -300.0f), glm::vec3(0.3f, 0.25f, 0.2f) },
    
    // 비활성화
    { glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f) },
    { glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f) }
};

std::vector<std::unique_ptr<GameObject>> world;
std::vector<GameMode*> gModeStack;