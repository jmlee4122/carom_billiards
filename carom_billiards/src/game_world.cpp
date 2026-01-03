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
#include "./objects/Ball.h"
#include "./objects/Collider.h"

void World::AddObject(std::shared_ptr<GameObject> o) {
	world.push_back(o);
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

bool World::CheckCollision(const std::string& pairName, GameObject* a, GameObject* b) {
	if (pairName == "ball:ball") {
		return World::Spheres(static_cast<Ball*>(a), static_cast<Ball*>(b));
	}
	else if (pairName == "ball:wall") {
		return World::SphereAndLine(static_cast<Ball*>(a), static_cast<Collider*>(b));
	}
	else {
		return false;
	}
}

bool World::Spheres(Ball* a, Ball* b) {
	glm::vec3 firstPos = a->GetPosition();
	glm::vec3 secondPos = b->GetPosition();

	float r = a->GetRadius();

	float distance = glm::length(firstPos - secondPos);
	if (distance <= 2.5 * r) {
		return true;
	}
	return false;
}

bool World::SphereAndLine(Ball* a, Collider* b) {
	glm::vec3 ballPos = a->GetPosition();
	float ballRadius = a->GetRadius();
	std::vector<Wall> walls = b->GetWalls();

	for (size_t wallIndex = 0; wallIndex < walls.size(); ++wallIndex) {
		auto& wall = walls[wallIndex];
		
		glm::vec2 ballPos2D = glm::vec2(ballPos.x, ballPos.z);
		glm::vec2 wallStart2D = glm::vec2(wall.start.x, wall.start.z);
		glm::vec2 wallEnd2D = glm::vec2(wall.end.x, wall.end.z);

		glm::vec2 wallDir = wallEnd2D - wallStart2D;
		float wallLengthSq = glm::dot(wallDir, wallDir);

		if (wallLengthSq < 0.0001f) continue;

		glm::vec2 startToBall = ballPos2D - wallStart2D;
		float t = glm::dot(startToBall, wallDir) / wallLengthSq;
		t = glm::clamp(t, 0.0f, 1.0f);
		
		glm::vec2 closestPoint = wallStart2D + t * wallDir;
		float distance = glm::length(ballPos2D - closestPoint);

		if (distance <= ballRadius) {
			b->SetIsCollision(wallIndex, true);
			return true;
		}
	}
	
	for (int wallIndex = 0; wallIndex < 4; wallIndex++) {
		b->SetIsCollision(wallIndex, false);
	}
	return false;
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
	for (auto& pair : gPairs) {
		if (pair.pairName == "ball:ball") {
			for (GameObject* obj : pair.objects[0]) {
				static_cast<Ball*>(obj)->ClearImpulse();
			}
		}
	}

	for (auto& pair : gPairs) {
		std::string name = pair.pairName;

		auto& groupA = pair.objects[0];
		auto& groupB = pair.objects[1];

		for (GameObject* a : groupA) {
			if (a == nullptr) continue;

			for (GameObject* b : groupB) {
				if (b == nullptr) continue;
				if (a == b) continue;
				if (World::CheckCollision(name, a, b)) {
					a->HandleCollision(name, b);
					b->HandleCollision(name, a);
				}
			}
		}
	}

	for (auto& pair : gPairs) {
		if (pair.pairName == "ball:ball") {
			for (GameObject* obj : pair.objects[0]) {
				static_cast<Ball*>(obj)->ApplyImpulse();
			}
		}
	}
}

void World::RemoveCollisionObject() {

}