#include <iostream>

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

#include "../MyStruct.h"
#include "../game_world.h"

#include "Collider.h"

Collider::Collider(Model* model) {
	this->objectName = "wall";

	this->max_x = model->vertices[0].x;
	this->max_y = model->vertices[0].y;
	this->max_z = model->vertices[0].z;

	this->min_x = model->vertices[0].x;
	this->min_y = model->vertices[0].y;
	this->min_z = model->vertices[0].z;

	if (model != nullptr) {
		for (int i = 0; i < model->vertex_count; i++) {
			glm::vec3 target = glm::vec3(model->vertices[i].x, model->vertices[i].y, model->vertices[i].z);
			this->max_x = std::max(max_x, target.x);
			this->max_y = std::max(max_y, target.y);
			this->max_z = std::max(max_z, target.z);

			this->min_x = std::min(min_x, target.x);
			this->min_y = std::min(min_y, target.y);
			this->min_z = std::min(min_z, target.z);
		}
	}

	World::AddCollisionPair("ball:wall", nullptr, this);
}

Collider::~Collider() {

}

void Collider::MakeWalls() {
	glm::vec3 p[4];

	p[0] = glm::vec3(min_x, max_y, min_z);
	p[1] = glm::vec3(max_x, max_y, min_z);
	p[2] = glm::vec3(max_x, max_y, max_z);
	p[3] = glm::vec3(min_x, max_y, max_z);

	for (int i = 0; i < 4; i++) {
		this->walls.push_back({ p[i], p[(i + 1) % 4] });
	}

	std::cout << "======= Wall Test =======" << std::endl;
	for (int i = 0; i < 4; i++) {
		std::cout << i << " start point : ";
		std::cout << this->walls[i].start.x << " " << this->walls[i].start.y << " " << this->walls[i].start.z << std::endl;
		std::cout << i << " end point   : ";
		std::cout << this->walls[i].end.x << " " << this->walls[i].end.y << " " << this->walls[i].end.z << std::endl;
	}
}

void Collider::Update(float dt) {

}

void Collider::Render(GLuint shaderID) {

}

std::string Collider::GetObjectName() {
	return this->objectName;
}