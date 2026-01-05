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
#include "./objects/Circle.h"

void PlayMode::Init() {
	this->isShiftPressed = false;
	Model* m = new Model;
	read_obj_file("./src/assets/models/billiard_table.obj", m);
	World::AddObject(std::make_shared<Table>(m));

	read_obj_file("./src/assets/models/billiard_plane.obj", m);
	World::AddObject(std::make_shared<Plane>(m));

	read_obj_file("./src/assets/models/billiard_ball.obj", m);
	float dia = 244.0f / 8.0f;
	float r = 3.275f;
	World::AddObject(std::make_shared<Ball>(m, glm::vec3(-2 * dia, 106 + r, 0), glm::vec3(0, 0, 0), "red"));
	World::AddObject(std::make_shared<Ball>(m, glm::vec3(2 * dia, 106 + r, 0), glm::vec3(0, 0, 0), "red"));
	World::AddObject(std::make_shared<Ball>(m, glm::vec3(2 * dia, 106 + r, -0.5 * dia), glm::vec3(0, 0, 0), "white"));
	World::AddObject(std::make_shared<Ball>(m, glm::vec3(-3 * dia, 106 + r, 0), glm::vec3(0, 0, 0), "yellow"));

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

	gBallSection = new Circle(1.0f, glm::vec3(1, 1, 1));
	gCuePoint = new Circle(0.14f, glm::vec3(1, 0, 0));

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

					glm::vec2 hitPos = gCuePoint->GetPosition();

					// 스핀 강도를 파워에 비례하게 증가 (끌림 효과 강화)
					float powerNormalized = glm::length(gShootPower * dir) / 100.0f; // 파워 정규화
					float spinFactor = 0.8f + powerNormalized * 0.5f; // 기본 8, 최대 13 정도

					glm::vec3 sideSpinAxis = glm::vec3(0, 1, 0);
					glm::vec3 topSpinAxis = glm::normalize(glm::cross(dir, sideSpinAxis));

					// Y축 방향(위아래)에만 스핀 팩터를 강하게 적용
					glm::vec3 angularVel = sideSpinAxis * -hitPos.x * spinFactor
						- topSpinAxis * hitPos.y * spinFactor;
					ball->SetAngularVelocity(angularVel);
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
	if (key == GLUT_KEY_SHIFT_L) {
		isShiftPressed = true;
		cameraMain->SetIsLeft(false);
		cameraMain->SetIsRight(false);
	}
	
	if (isShiftPressed) {
		switch (key) {
		case GLUT_KEY_LEFT:
			gCuePoint->SetIsLeft(true);
			break;
		case GLUT_KEY_RIGHT:
			gCuePoint->SetIsRight(true);
			break;
		case GLUT_KEY_UP:
			gCuePoint->SetIsUp(true);
			break;
		case GLUT_KEY_DOWN:
			gCuePoint->SetIsDown(true);
			break;
		}
	}
	else {
		switch (key) {
		case GLUT_KEY_LEFT:
			cameraMain->SetIsRight(true);
			break;
		case GLUT_KEY_RIGHT:
			cameraMain->SetIsLeft(true);
			break;
		}
	}
}

void PlayMode::HandleSpecialKeyUp(int key, int x, int y) {
	if (key == GLUT_KEY_SHIFT_L) {
		isShiftPressed = false;
		gCuePoint->SetIsLeft(false);
		gCuePoint->SetIsRight(false);
		gCuePoint->SetIsUp(false);
		gCuePoint->SetIsDown(false);
	}

	if (isShiftPressed) {
		switch (key) {
		case GLUT_KEY_LEFT:
			gCuePoint->SetIsLeft(false);
			break;
		case GLUT_KEY_RIGHT:
			gCuePoint->SetIsRight(false);
			break;
		case GLUT_KEY_UP:
			gCuePoint->SetIsUp(false);
			break;
		case GLUT_KEY_DOWN:
			gCuePoint->SetIsDown(false);
			break;
		}
	}
	else {
		switch (key) {
		case GLUT_KEY_LEFT:
			cameraMain->SetIsRight(false);
			break;
		case GLUT_KEY_RIGHT:
			cameraMain->SetIsLeft(false);
			break;
		}
	}
}

void PlayMode::Update(float dt) {
	World::Update(dt);
	World::HandleCollisions();
	cameraMain->Update();
	gBallSection->Update();
	gCuePoint->Update();
	SetScore();
}

void PlayMode::RenderText(float x, float y, const std::string& text, void* font) {
	glUseProgram(0);
	
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, SCR_WIDTH, 0, SCR_HEIGHT);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	glColor3f(1.0f, 1.0f, 1.0f);
	glRasterPos2f(x, y);

	for (char c : text) {
		glutBitmapCharacter(font, c);
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
}

void PlayMode::Draw(GLuint shaderID) {
	World::Render(shaderID);
}

void PlayMode::DrawUI() {
	std::string whiteScoreText = "White: " + std::to_string(gScore.whiteScore);
	std::string yellowScoreText = "Yellow: " + std::to_string(gScore.yellowScore);
	std::string turnText = "Turn: " + gScore.cueBallColor;

	RenderText(10, SCR_HEIGHT - 30, whiteScoreText, GLUT_BITMAP_HELVETICA_18);
	RenderText(10, SCR_HEIGHT - 60, yellowScoreText, GLUT_BITMAP_HELVETICA_18);
	RenderText(10, SCR_HEIGHT - 90, turnText, GLUT_BITMAP_HELVETICA_18);
}