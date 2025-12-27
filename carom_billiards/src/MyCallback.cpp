#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm> 
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

#include "MyCallback.h"
#include "MyExtern.h"
#include "MyStruct.h"
#include "MyUtils.h"
#include "play_mode.h"

#include "./objects/GameObject.h"

GLvoid DrawScene() {
    static bool firstFrame = true;
    if (firstFrame) {
        std::cout << "\n========== RENDERING START ==========" << std::endl;
        std::cout << "Shadow Shader ID: " << shadowShaderProgramID << std::endl;
        std::cout << "Main Shader ID: " << shaderProgramID << std::endl;
        std::cout << "Light[0] Position: (" << gLights[0].position.x << ", " 
                  << gLights[0].position.y << ", " << gLights[0].position.z << ")" << std::endl;
        std::cout << "Camera Position: (" << cameraPos.x << ", " 
                  << cameraPos.y << ", " << cameraPos.z << ")" << std::endl;
        firstFrame = false;
    }

    // ====== Pass 1: Shadow Map 렌더링 ======
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glClear(GL_DEPTH_BUFFER_BIT);

    glUseProgram(shadowShaderProgramID);

    // Light Space Matrix 범위 확대 (전체 씬을 커버하도록)
    glm::mat4 lightProjection = glm::ortho(-500.0f, 500.0f, -500.0f, 500.0f, 1.0f, 1000.0f);
    glm::mat4 lightView = glm::lookAt(gLights[0].position, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    lightSpaceMatrix = lightProjection * lightView;

    glUniformMatrix4fv(glGetUniformLocation(shadowShaderProgramID, "lightSpaceMatrix"),
        1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

    for (auto r : gModeStack) {
        r->Draw(shadowShaderProgramID);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // ====== Pass 2: 최종 렌더링 ======
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glClearColor(0.05f, 0.05f, 0.1f, 1.0f); // 약간 어두운 파란 배경
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgramID);

    // Shadow Map 텍스처 바인딩
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glUniform1i(glGetUniformLocation(shaderProgramID, "shadowMap"), 0);

    // Light Space Matrix 전송
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "lightSpaceMatrix"),
        1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

    // 카메라/투영 행렬 전송
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "view"), 1, GL_FALSE, glm::value_ptr(gViewMat));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "projection"), 1, GL_FALSE, glm::value_ptr(gProjMat));
    glUniform3fv(glGetUniformLocation(shaderProgramID, "viewPos"), 1, glm::value_ptr(cameraPos));

    // 조명 정보 전송
    for (unsigned int i = 0; i < gLightsCount; i++) {
        std::string posName = "lights[" + std::to_string(i) + "].position";
        std::string colName = "lights[" + std::to_string(i) + "].color";

        glUniform3fv(glGetUniformLocation(shaderProgramID, posName.c_str()), 1, glm::value_ptr(gLights[i].position));
        glUniform3fv(glGetUniformLocation(shaderProgramID, colName.c_str()), 1, glm::value_ptr(gLights[i].color));
    }
    glUniform1i(glGetUniformLocation(shaderProgramID, "lightCount"), gLightsCount);

    // OpenGL 에러 체크
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "[DrawScene] OpenGL Error after uniforms: " << err << std::endl;
    }

    // 객체 렌더링
    for (auto r : gModeStack) {
        r->Draw(shaderProgramID);
    }
    
    glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {

}

GLvoid Keyboard(unsigned char key, int x, int y) {

}

GLvoid KeyboardUp(unsigned char key, int x, int y) {

}

GLvoid SpecialKey(int key, int x, int y) {
	glutPostRedisplay();
}

GLvoid SpecialKeyUp(int key, int x, int y) {
	glutPostRedisplay();
}

GLvoid Timer(int value) {

}

GLvoid Mouse(int button, int state, int x, int y) {

}
GLvoid MouseMotion(int x, int y) {

}

GLvoid PassiveMotion(int x, int y) {

}