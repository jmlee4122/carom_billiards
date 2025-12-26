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

    // --- 셰이더 로드 및 컴파일 ---
    // 경로가 길면 변수로 빼는 게 보기 좋습니다.
    make_vertexShaders("./src/assets/shaders/vertex.glsl");
    make_fragmentShaders("./src/assets/shaders/fragment.glsl");
    shaderProgramID = make_shaderProgram();
    glEnable(GL_DEPTH_TEST);
    return true; // 성공
}