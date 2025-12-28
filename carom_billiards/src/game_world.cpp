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

void World::Update(float dt) {
	for (const auto& obj : world) {
		obj->Update(dt);
	}
}

void World::Render(GLuint shaderID) {
	for (const auto& obj : world) {
		obj->Render(shaderID);
	}
}

void World::Clear() {

}

void World::Collide() {

}

void World::AddCollisionPair(const std::string& pairName, GameObject* a, GameObject* b) {
	for (int i = 0; i < gPairs.size(); i++) {
		if (pairName == gPairs[i].pairName) {
			if (a != nullptr) gPairs[i].objects[0].push_back(a);
			if (b != nullptr) gPairs[i].objects[1].push_back(b);
			return;
		}
	}
	CollisionPair pair;
	pair.pairName = pairName;
	if (a != nullptr) pair.objects[0].push_back(a);
	if (b != nullptr) pair.objects[1].push_back(b);
	gPairs.push_back(pair);
}

void World::HandleCollisions() {

}

void World::RemoveCollisionObject() {

}