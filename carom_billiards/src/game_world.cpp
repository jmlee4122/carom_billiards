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
	// 동일한 공 충돌 검사 제외
	return false;
}

bool World::SphereAndLine(Ball* a, Collider* b) {
	glm::vec3 ballPos = a->GetPosition();
	float ballRadius = a->GetRadius();
	std::vector<Wall> walls = b->GetWalls();

	for (auto& wall : walls) {
		// start point -> ball position
		glm::vec3 startBall = ballPos - wall.start;
		// start point -> end point
		glm::vec3 startEnd = wall.end - wall.start;
		// dot (startBall -> startEnd)
		float dotResult = glm::dot(startEnd, startBall);
		// length squared of startEnd
		float lengthSq = startEnd.x * startEnd.x + startEnd.y * startEnd.y + startEnd.z * startEnd.z;
		// start point -> closest point
		glm::vec3 startClosest = (dotResult / lengthSq) * startEnd;
		// closest point position
		glm::vec3 closestPoint = wall.start + startClosest;
		// ball position -> closest point
		glm::vec3 ballClosest = closestPoint - ballPos;
		// lenght (ball position -> closest point)
		float distance = glm::length(ballClosest);

		if (distance <= ballRadius) {
			glm::vec3 nol = glm::normalize(-ballClosest);
			b->SetCollisionNormal(nol);
			return true;
		}
	}
	b->SetCollisionNormal(glm::vec3(0, 0, 0));
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
		std::string name = pair.pairName;

		auto& groupA = pair.objects[0];
		auto& groupB = pair.objects[1];

		for (GameObject* a : groupA) {
			if (a == nullptr) continue;

			for (GameObject* b : groupB) {
				if (b == nullptr) continue;

				if (World::CheckCollision(name, a, b)) {
					a->HandleCollision(name, b);
					b->HandleCollision(name, a);
				}
			}
		}
	}
}

void World::RemoveCollisionObject() {

}