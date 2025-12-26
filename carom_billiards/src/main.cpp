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

#include "./objects/GameObject.h"
#include "./objects/Table.h"

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	srand(static_cast<unsigned int>(time(NULL))); // 시드 초기화
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(SCR_WIDTH, SCR_HEIGHT);
	glutCreateWindow("Carom Billiards");
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();
	//--- 세이더 읽어와서 세이더 프로그램 만들기: 사용자 정의함수 호출
	make_vertexShaders("D:\\carom_billiards\\carom_billiards\\carom_billiards\\src\\assets\\shaders\\vertex.glsl"); //--- 버텍스 세이더 만들기
	make_fragmentShaders("D:\\carom_billiards\\carom_billiards\\carom_billiards\\src\\assets\\shaders\\fragment.glsl"); //--- 프래그먼트 세이더 만들기
	shaderProgramID = make_shaderProgram(); //--- 세이더 프로그램 만들기

	glEnable(GL_DEPTH_TEST);

	// *** TEST ***
	PlayMode::Init();
	// *** TEST ***

	//glutSetCursor(GLUT_CURSOR_NONE); // 커서를 보이지 않게 함
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