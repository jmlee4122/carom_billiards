#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
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
#include "./objects/CameraMain.h"

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

    // Pass 1: Shadow Map rendering
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glClear(GL_DEPTH_BUFFER_BIT);

    glUseProgram(shadowShaderProgramID);

    glm::mat4 lightProjection = glm::ortho(-500.0f, 500.0f, -500.0f, 500.0f, 1.0f, 1000.0f);
    glm::mat4 lightView = glm::lookAt(gLights[0].position, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    lightSpaceMatrix = lightProjection * lightView;

    glUniformMatrix4fv(glGetUniformLocation(shadowShaderProgramID, "lightSpaceMatrix"),
        1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

    if (!gModeStack.empty()) {
        for (auto r : gModeStack) {
            r->Draw(shadowShaderProgramID);
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Pass 2 : final rendering
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgramID);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glUniform1i(glGetUniformLocation(shaderProgramID, "shadowMap"), 0);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "lightSpaceMatrix"),
        1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

    glm::mat4 viewMat = cameraMain->GetViewMat();

    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "view"), 1, GL_FALSE, glm::value_ptr(viewMat));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "projection"), 1, GL_FALSE, glm::value_ptr(gProjMat));
    glUniform3fv(glGetUniformLocation(shaderProgramID, "viewPos"), 1, glm::value_ptr(cameraPos));

    for (unsigned int i = 0; i < gLightsCount; i++) {
        std::string posName = "lights[" + std::to_string(i) + "].position";
        std::string colName = "lights[" + std::to_string(i) + "].color";

        glUniform3fv(glGetUniformLocation(shaderProgramID, posName.c_str()), 1, glm::value_ptr(gLights[i].position));
        glUniform3fv(glGetUniformLocation(shaderProgramID, colName.c_str()), 1, glm::value_ptr(gLights[i].color));
    }
    glUniform1i(glGetUniformLocation(shaderProgramID, "lightCount"), gLightsCount);

    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "[DrawScene] OpenGL Error after uniforms: " << err << std::endl;
    }

    if (!gModeStack.empty()) {
        for (auto r : gModeStack) {
            r->Draw(shaderProgramID);
        }
    }

    // Pass 3: mini map rendering
    int miniMapWidth = SCR_WIDTH / 3.0f;
    int miniMapHeight = SCR_HEIGHT / 3.0f;
    glViewport(SCR_WIDTH - miniMapWidth, SCR_HEIGHT - miniMapHeight, miniMapWidth, miniMapHeight);

    glClear(GL_DEPTH_BUFFER_BIT);

    float orthoSize = 150.0f;
    glm::mat4 miniMapProj = glm::ortho(-orthoSize, orthoSize, -orthoSize, orthoSize, 1.0f, 1000.0f);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "view"), 1, GL_FALSE, glm::value_ptr(gSubViewMat));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "projection"), 1, GL_FALSE, glm::value_ptr(miniMapProj));

    if (!gModeStack.empty()) {
        for (auto r : gModeStack) {
            r->Draw(shaderProgramID);
        }
    }

    glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {

}

GLvoid Keyboard(unsigned char key, int x, int y) {
    if (!gModeStack.empty()) {
        gModeStack.back()->HandleKey(key, x, y);
    }
    glutPostRedisplay();
}

GLvoid KeyboardUp(unsigned char key, int x, int y) {

}

GLvoid SpecialKey(int key, int x, int y) {
    if (!gModeStack.empty()) {
        gModeStack.back()->HandleSpecialKey(key, x, y);
    }
	glutPostRedisplay();
}

GLvoid SpecialKeyUp(int key, int x, int y) {
    if (!gModeStack.empty()) {
        gModeStack.back()->HandleSpecialKeyUp(key, x, y);
    }
	glutPostRedisplay();
}

GLvoid Timer(int value) {
    int nextTime = glutGet(GLUT_ELAPSED_TIME);
    float deltaTime = (float)(nextTime - gCurrTime) / 1000.0f;

    const float MAX_DELTA_TIME = 0.1f;
    if (deltaTime > MAX_DELTA_TIME) {
        deltaTime = MAX_DELTA_TIME;
    }

    if (!gModeStack.empty()) {
        for (auto r : gModeStack) {
            r->Update(deltaTime);
        }
    }

    gCurrTime = nextTime;

    glutPostRedisplay();
    glutTimerFunc(8, Timer, 0); // ¾à 120 FPS
}

GLvoid Mouse(int button, int state, int x, int y) {

}
GLvoid MouseMotion(int x, int y) {

}

GLvoid PassiveMotion(int x, int y) {

}