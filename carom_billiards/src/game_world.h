#pragma once

#include <memory>

class GameObject;
class Ball;
class Collider;

namespace World
{
	void AddObject(std::shared_ptr<GameObject> o);
	void AddObjects();
	void RemoveObject();

	void Update(float dt);
	void Render(GLuint shaderID);
	void Clear();

	bool CheckCollision(const std::string& pairName, GameObject* a, GameObject* b);
	bool Spheres(Ball* a, Ball* b);
	bool SphereAndLine(Ball* a, Collider* b);
	void AddCollisionPair(const std::string& pairName, GameObject* a, GameObject* b);
	void HandleCollisions();
	void RemoveCollisionObject();
}