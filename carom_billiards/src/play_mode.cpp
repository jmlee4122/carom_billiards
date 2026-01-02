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
	World::AddObject(std::make_shared<Ball>(m, glm::vec3(2 * dia, 110, -0.5 * dia), 50.0f * glm::vec3(-pixel_per_cm * 8.0, 0, pixel_per_cm * 1.3f), "white"));
	World::AddObject(std::make_shared<Ball>(m, glm::vec3(-3 * dia, 110, 0), glm::vec3(0, 0, 0), "yellow"));

	read_obj_file("./src/assets/models/billiard_collider.obj", m);
	gCollider = new Collider(m);
	gCollider->MakeWalls();

	// PrintCollisionPairs();

	// init global score
	gScore.cueBallColor = "white";
	gScore.whiteScore = 0;
	gScore.yellowScore = 0;

	// init global collision information
	gCollisionInfo.cntRed = 0;
	gCollisionInfo.cntWhite = 0;
	gCollisionInfo.cntYellow = 0;

	gCurrTime = glutGet(GLUT_ELAPSED_TIME);
}

void PlayMode::Cleanup() {

}

void PlayMode::HandleEvents(unsigned char key, int x, int y) {

}

void PlayMode::Update(float dt) {
	World::Update(dt);
	World::HandleCollisions();

	SetScore();
}

void PlayMode::Draw(GLuint shaderID) {
	World::Render(shaderID);
}
