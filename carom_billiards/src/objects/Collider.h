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

private:
	float max_x, max_y, max_z;
	float min_x, min_y, min_z;
	std::vector<Wall> walls;
};