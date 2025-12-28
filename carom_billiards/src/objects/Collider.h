#pragma once

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/glm/glm.hpp>

struct Model;

class Collider
{
public:
	Collider(Model* model);
	~Collider();
	void PrintMaxAndMin() const;
private:
	float max_x, max_z;
	float min_x, min_y, min_z;
};

