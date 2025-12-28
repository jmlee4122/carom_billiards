#pragma once

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/glm/glm.hpp>

#include "GameObject.h"

struct Model;
struct Wall;

class Collider : public GameObject
{
public:
	Collider(Model* model);
	~Collider() override;
	void MakeWalls();
	void Update(float dt) override;
	void Render(GLuint shaderID) override;
	std::string GetObjectName() override;
	void HandleCollision(std::string name, GameObject* other) override;

private:
	float max_x, max_y, max_z;
	float min_x, min_y, min_z;
	std::vector<Wall> walls;

	std::string objectName;
};