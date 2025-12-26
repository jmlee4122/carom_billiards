#include <iostream>
#include <vector>
#include <memory>

#include "play_mode.h"
#include "game_world.h"
#include "MyUtils.h"
#include "MyStruct.h"

#include "./objects/Table.h"

void PlayMode::HandleEvents() {

}

void PlayMode::Init() {
	Model* m = new Model;
	read_obj_file("D:\\carom_billiards\\carom_billiards\\carom_billiards\\src\\assets\\models\\billiard_table.obj", m);
	print_model_info(m);
	World::AddObject(std::make_unique<Table>(m));
}

void PlayMode::Update() {

}