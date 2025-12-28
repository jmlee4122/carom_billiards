#pragma once

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/glm/glm.hpp>

struct Model;
struct Wall;

class Collider
{
public:
	Collider(Model* model);
	~Collider();
	void PrintMaxAndMin() const;
	void MakeWalls();

private:
	float max_x, max_y, max_z;
	float min_x, min_y, min_z;
	std::vector<Wall> walls;
};