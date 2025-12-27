#include <iostream>
#include <vector>
#include <memory>

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

#include "MyExtern.h"
#include "MyUtils.h"
#include "MyStruct.h"
#include "MyCallback.h"
#include "game_world.h"
#include "play_mode.h"
#include "game_framework.h"

#include "./objects/GameObject.h"
#include "./objects/Table.h"

void Framework::Run(int argc, char** argv, GameMode* startMode) {
    if (!Initialize(argc, argv)) {
        return; // 초기화 실패 시 실행하지 않음
    }

    PushMode(startMode);

    glutDisplayFunc(DrawScene); //--- 출력 콜백 함수
    glutReshapeFunc(Reshape);
    /*glutKeyboardFunc(Keyboard);
    glutKeyboardUpFunc(KeyboardUp);
    glutSpecialFunc(SpecialKey);
    glutSpecialUpFunc(SpecialKeyUp);
    glutMouseFunc(Mouse);
    glutPassiveMotionFunc(PassiveMotion);
    glutTimerFunc(0, Timer, 0);*/
    glutMainLoop();

    glDisable(GL_DEPTH_TEST);
}

void Framework::ChangeMode(GameMode* mode) {

}
void Framework::PushMode(GameMode* mode) {
    gModeStack.push_back(mode);
    mode->Init();
}
void Framework::PopMode() {

}
void Framework::quit() {

}

bool Framework::Initialize(int argc, char** argv) {
    srand(static_cast<unsigned int>(time(NULL)));

    // --- 윈도우 생성 (GLUT) ---
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(SCR_WIDTH, SCR_HEIGHT);
    glutCreateWindow("Carom Billiards");

    // --- GLEW 초기화 ---
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        return false; // 에러 처리
    }

    glEnable(GL_DEPTH_TEST);

    glGenFramebuffers(1, &depthMapFBO);
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
        SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // --- 셰이더 로드 및 컴파일 ---
    // 경로가 길면 변수로 빼는 게 보기 좋습니다.
    make_vertexShaders("./src/assets/shaders/shadow_vertex.glsl", shadowVertexShader);
    make_fragmentShaders("./src/assets/shaders/shadow_fragment.glsl", shadowFragmentShader);
    shadowShaderProgramID = make_shaderProgram(shadowVertexShader, shadowFragmentShader);

    make_vertexShaders("./src/assets/shaders/vertex.glsl", vertexShader);
    make_fragmentShaders("./src/assets/shaders/fragment.glsl", fragmentShader);
    shaderProgramID = make_shaderProgram(vertexShader, fragmentShader);

    return true; // 성공
}