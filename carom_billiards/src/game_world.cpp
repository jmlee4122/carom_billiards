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

#include "./objects/GameObject.h"

void World::AddObject(std::unique_ptr<GameObject> o) {
	world.push_back(std::move(o));
}
void World::AddObjects() {

}
void World::RemoveObject() {

}

void World::Update() {

}
void World::Render() {
	for (const auto& obj : world) {
		obj->Render();
	}
}
void World::Clear() {

}

void World::Collide() {

}
void World::AddCollisionPair() {

}
void World::HandleCollisions() {

}
void World::RemoveCollisionObject() {

}