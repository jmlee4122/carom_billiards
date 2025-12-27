#pragma once

#include <memory>

class GameObject;

namespace World
{
	void AddObject(std::unique_ptr<GameObject> o);
	void AddObjects();
	void RemoveObject();

	void Update();
	void Render(GLuint shaderID);
	void Clear();

	void Collide();
	void AddCollisionPair();
	void HandleCollisions();
	void RemoveCollisionObject();
}