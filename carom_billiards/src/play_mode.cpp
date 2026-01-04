#include <iostream>
#include <vector>
#include <memory>
#include <string>

#include "play_mode.h"
#include "game_world.h"
#include "MyUtils.h"
#include "MyStruct.h"
#include "MyExtern.h"

#include "./objects/Table.h"
#include "./objects/Plane.h"
#include "./objects/Ball.h"
#include "./objects/Collider.h"
#include "./objects/CameraMain.h"

void PlayMode::Init() {
	Model* m = new Model;
	read_obj_file("./src/assets/models/billiard_table.obj", m);
	World::AddObject(std::make_shared<Table>(m));

	read_obj_file("./src/assets/models/billiard_plane.obj", m);
	World::AddObject(std::make_shared<Plane>(m));

	read_obj_file("./src/assets/models/billiard_ball.obj", m);
	float dia = 244.0f / 8.0f;
	World::AddObject(std::make_shared<Ball>(m, glm::vec3(-2 * dia, 110, 0), glm::vec3(0, 0, 0), "red"));
	World::AddObject(std::make_shared<Ball>(m, glm::vec3(2 * dia, 110, 0), glm::vec3(0, 0, 0), "red"));
	World::AddObject(std::make_shared<Ball>(m, glm::vec3(2 * dia, 110, -0.5 * dia), glm::vec3(0, 0, 0), "white"));
	World::AddObject(std::make_shared<Ball>(m, glm::vec3(-3 * dia, 110, 0), glm::vec3(0, 0, 0), "yellow"));

	read_obj_file("./src/assets/models/billiard_collider.obj", m);
	gCollider = new Collider(m);
	gCollider->MakeWalls();

	// init global score
	gScore.cueBallColor = "white";
	gScore.whiteScore = 0;
	gScore.yellowScore = 0;

	// init global collision information
	gCollisionInfo.cntRed = 0;
	gCollisionInfo.cntWhite = 0;
	gCollisionInfo.cntYellow = 0;

	cameraMain = new CameraMain();
	cameraMain->SetAt();

	gCurrTime = glutGet(GLUT_ELAPSED_TIME);
}

void PlayMode::Cleanup() {

}

void PlayMode::HandleKey(unsigned char key, int x, int y) {
	if ('1' <= key && key <= '9') {
		float n = key - '0';
		gShootPower = 10.0f * pixel_per_cm * n * n;
	}
	switch (key) {
	case ' ':
		if (!gHasReady) break;
		gHasReady = false;
		for (const auto& r : world) {
			if (r->GetObjectName() == "ball") {
				Ball* ball = static_cast<Ball*>(r.get());
				if (ball->GetColor() == gScore.cueBallColor) {
					glm::vec3 dir = GetShootDir();
					ball->SetVelocity(gShootPower * dir);
				}
			}
		}
		break;
	case 'q':
		glutLeaveMainLoop();
	}
	glutSwapBuffers();
}

void PlayMode::HandleSpecialKey(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		cameraMain->SetIsRight(true);
		break;
	case GLUT_KEY_RIGHT:
		cameraMain->SetIsLeft(true);
		break;
	}
}

void PlayMode::HandleSpecialKeyUp(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		cameraMain->SetIsRight(false);
		break;
	case GLUT_KEY_RIGHT:
		cameraMain->SetIsLeft(false);
		break;
	}
}

void PlayMode::Update(float dt) {
	World::Update(dt);
	World::HandleCollisions();
	cameraMain->Update();
	SetScore();
}

void PlayMode::RenderText(float x, float y, const std::string& text, void* font) {
	// 셰이더 비활성화
	glUseProgram(0);
	
	// 현재 뷰포트 저장
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	
	// 전체 화면으로 뷰포트 설정
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	
	// Projection 행렬을 2D로 설정
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, SCR_WIDTH, 0, SCR_HEIGHT);

	// ModelView 행렬 초기화
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// 텍스트 렌더링에 필요한 설정
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	// 텍스트 색상 설정 (흰색)
	glColor3f(1.0f, 1.0f, 1.0f);
	glRasterPos2f(x, y);

	// 문자 하나씩 렌더링
	for (char c : text) {
		glutBitmapCharacter(font, c);
	}

	// 원래 상태로 복원
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	// 행렬 복원
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	
	// 뷰포트 복원
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
}

void PlayMode::Draw(GLuint shaderID) {
	World::Render(shaderID);
}

void PlayMode::DrawUI() {
	// 스코어 렌더링 (셰이더 렌더링 후 호출)
	std::string whiteScoreText = "White: " + std::to_string(gScore.whiteScore);
	std::string yellowScoreText = "Yellow: " + std::to_string(gScore.yellowScore);
	std::string turnText = "Turn: " + gScore.cueBallColor;

	RenderText(10, SCR_HEIGHT - 30, whiteScoreText, GLUT_BITMAP_HELVETICA_18);
	RenderText(10, SCR_HEIGHT - 60, yellowScoreText, GLUT_BITMAP_HELVETICA_18);
	RenderText(10, SCR_HEIGHT - 90, turnText, GLUT_BITMAP_HELVETICA_18);
}