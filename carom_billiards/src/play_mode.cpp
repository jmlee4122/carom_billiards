#include <iostream>
#include <vector>
#include <memory>

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
	switch (key) {
	case 'r':
		gHasReady = false;
		for (const auto& r : world) {
			if (r->GetObjectName() == "ball") {
				Ball* ball = static_cast<Ball*>(r.get());
				if (ball->GetColor() == gScore.cueBallColor) {
					ball->SetVelocity(glm::vec3(50.0f * glm::vec3(-pixel_per_cm * 8.0, 0, pixel_per_cm * 1.3f)));
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
		cameraMain->SetIsLeft(true);
		break;
	case GLUT_KEY_RIGHT:
		cameraMain->SetIsRight(true);
		break;
	}
}

void PlayMode::HandleSpecialKeyUp(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		cameraMain->SetIsLeft(false);
		break;
	case GLUT_KEY_RIGHT:
		cameraMain->SetIsRight(false);
		break;
	}
}

void PlayMode::Update(float dt) {
	World::Update(dt);
	World::HandleCollisions();
	cameraMain->Update();
	SetScore();
}

void PlayMode::Draw(GLuint shaderID) {
	World::Render(shaderID);
}
