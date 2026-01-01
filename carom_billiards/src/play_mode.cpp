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
	World::AddObject(std::make_shared<Ball>(m, glm::vec3(0, 110, 0)));
	World::AddObject(std::make_shared<Ball>(m, glm::vec3(100, 110, 50)));

	read_obj_file("./src/assets/models/billiard_collider.obj", m);
	gCollider = new Collider(m);
	gCollider->MakeWalls();

	PrintCollisionPairs();

	gCurrTime = glutGet(GLUT_ELAPSED_TIME);
}

void PlayMode::Cleanup() {

}

void PlayMode::HandleEvents(unsigned char key, int x, int y) {

}

void PlayMode::Update(float dt) {
	World::Update(dt);
	World::HandleCollisions();
}

void PlayMode::Draw(GLuint shaderID) {
	World::Render(shaderID);
}
