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

void PlayMode::Init() {
	Model* m = new Model;
	read_obj_file("./src/assets/models/billiard_table.obj", m);
	print_model_info(m);
	World::AddObject(std::make_unique<Table>(m));

	read_obj_file("./src/assets/models/billiard_plane.obj", m);
	print_model_info(m);
	World::AddObject(std::make_unique<Plane>(m));

	read_obj_file("./src/assets/models/billiard_ball.obj", m);
	print_model_info(m);
	World::AddObject(std::make_unique<Ball>(m));
}

void PlayMode::Cleanup() {

}

void PlayMode::HandleEvents(unsigned char key, int x, int y) {

}

void PlayMode::Update(float dt) {
	World::Update(dt);
}

void PlayMode::Draw(GLuint shaderID) {
	World::Render(shaderID);
}
