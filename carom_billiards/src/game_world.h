#pragma once

#include <memory>

class GameObject;

namespace World
{
	void AddObject(std::unique_ptr<GameObject> o);
	void AddObjects();
	void RemoveObject();

	void Update(float dt);
	void Render(GLuint shaderID);
	void Clear();

	void Collide();
	void AddCollisionPair(const std::string& pairName, GameObject* a, GameObject* b);
	void HandleCollisions();
	void RemoveCollisionObject();
}